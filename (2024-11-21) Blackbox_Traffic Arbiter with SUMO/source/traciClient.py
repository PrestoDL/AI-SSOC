import traci
import sumolib
import socket
import threading
import time as _t
import pickle

'''
def update_lane_weights():
    edge_ids = traci.edge.getIDList()
    for edge_id in edge_ids:
        lane_count = traci.edge.getLaneNumber(edge_id)
        for i in range(lane_count):
            lane_id = f"{edge_id}_{i}"
            stopped_vehicle_count = sum(1 for v in traci.lane.getLastStepVehicleIDs(lane_id) if traci.vehicle.getSpeed(v) < 5)
            if stopped_vehicle_count > 0:
                # Reduce speed limit to increase lane cost
                new_speed = max(1.0, 300 / (30 + stopped_vehicle_count))  # Example: base speed 50 km/h reduced based on vehicle count
                traci.lane.setMaxSpeed(lane_id, new_speed)
            else:
                traci.lane.setMaxSpeed(lane_id, 10)
    
    # Re-route all vehicles based on updated travel times
    for vid in traci.vehicle.getIDList():
        traci.vehicle.rerouteTraveltime(vid)
'''







# edge의 가중치들은 이중 딕셔너리로 관리되어야 할 것 같음.
# {VehicleID : getRoute(list)} 딕셔너리와, {EdgeID : Count} 딕셔너리로 말이다

def update_edge_weights(routeSpeed, laneSpeed):
    edge_ids = traci.edge.getIDList()
    for edge_id in edge_ids:
        #vehicle_count = traci.edge.getWaitingTime(edge_id)
        #if vehicle_count != 0 : print(vehicle_count)
        weight = 1# + vehicle_count * 0.05
        #if (weight != 1): print(edge_id, min(weight, 10))
        traci.edge.setEffort(edge_id, min(weight, 10))#, traci.simulation.getTime() - 100, traci.simulation.getTime())
    
    for vid in traci.vehicle.getIDList():
        #traci.vehicle.rerouteTraveltime(vid)
        if traci.vehicle.getSpeed(vid) > routeSpeed :
            traci.vehicle.rerouteEffort(vid)
        nowRoute = traci.vehicle.getRoute(vid)
        nowEdge = traci.vehicle.getRoadID(vid)

        
        if ((traci.vehicle.getSpeed(vid) == 0) and (traci.vehicle.getWaitingTime(vid) > laneSpeed)):
                lane_id = traci.vehicle.getLaneID(vid)
                links = traci.lane.getLinks(lane_id)
                chosen_maneuver = 1  # 0: straight, 1: right turn, 2: left turn
                chosen_link = None

                for link in links:
                    if link[1] == chosen_maneuver:
                        chosen_link = link
                        break
                
                if chosen_link:
                    to_lane_id = chosen_link[0]
                    to_edge_id = traci.lane.getEdgeID(to_lane_id)
                    
                    # Set the vehicle's route to the chosen edge
                    route = traci.simulation.findRoute(traci.vehicle.getRoadID(vid), to_edge_id).edges
                    traci.vehicle.setRoute(vid, route)
        else:
            for route in nowRoute:
                traci.edge.setEffort(route, traci.edge.getEffort(route, traci.simulation.getTime()) + 0.5)

        traci.vehicle.setEffort(vid, nowEdge, 100000)
    #traci.edge.setEffort(nowEdge, min((traci.edge.getEffort(nowEdge, traci.simulation.getTime()) + 0.5 * traci.edge.getWaitingTime(nowEdge)), 20))
       



step = threading.Event()


def connectServer(vid, ip, port):
    try:
        #print(vid)
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((ip, port))

        prevRoadID = traci.vehicle.getRoadID(vid)

        while True:
            # Check if the vehicle is still in the simulation
            if vid not in traci.vehicle.getIDList():
                break

            step.wait()

            nowRoadID = traci.vehicle.getRoadID(vid)
            if (nowRoadID != prevRoadID): # Detect vehicle's edge-to-edge change
                nowRoute = traci.vehicle.getRoute(vid)
                serialRoute = pickle.dumps(nowRoute)
                s.send(serialRoute)
    
                newWeights = s.recv(2048)
                weightList = list(zip(traci.edge.getIDList(), newWeights))
                for weights in weightList:
                    if traci.edge.getEffort(weights[0], traci.simulation.getTime()) >= 100000:
                        traci.edge.setEffort(weights[0], weights[1], traci.simulation.getTime())
                traci.vehicle.rerouteEffort(vid)

            prevRoadID = nowRoadID

            #traci.vehicle.setEffort(vid, traci.vehicle.getRoadID(vid), 100000)  중복 경로 설정 불가
            
            step.clear()

        s.close()

        #message = f'Client {vid} disconnected from server'
        #print(message)

    except Exception as e:
        print(f'Client {id} failed to connect : {e}')



def simulation(routeSpeed, routeFreq, laneSpeed):

    # Directory
    sumo_binary = "C:/Program Files (x86)/Eclipse/Sumo/bin/sumo"
    net_dir = "./ql1.net.xml"
    sumocfg_dir = "./test.sumocfg"
    route_dir = "./route.rou.xml"
    sumolib.net.readNet(net_dir)

    # Run SUMO
    sumo_cmd = [sumo_binary, "-c", sumocfg_dir, "-r", route_dir, "--junction-taz", "--no-warnings", "--random"]
    traci.start(sumo_cmd)

    # Set variables
    time = 0
    distance = 0



    ip = input("Connect Address : ")
    port = int(input("Connecting Port : "))

    threads = []

    
    calcTime_s = _t.time()

    # Running ...
    while traci.simulation.getMinExpectedNumber() > 0:
        traci.simulationStep()
        time += 1
        
        vehicle_ids = traci.vehicle.getIDList()
        for veh_id in vehicle_ids:
            distance += traci.vehicle.getDistance(veh_id)

        edge_ids = traci.edge.getIDList()
        for edge_id in edge_ids:
            traci.edge.setEffort(edge_id, 1)


        # vehicle 생성 때마다 socket thread를 connect
        vehicles_departed = traci.simulation.getDepartedIDList()
        for vid in vehicles_departed:
            t = threading.Thread(target=connectServer, args=(vid, ip, port))
            threads.append(t)
            t.start()
        
        # map에 존재하는 vehicle에 대해 각자의 route 서버로 전송, weight를 반환
        # event driven programming으로 변환 필요할것같다. 마냥 step만 줄게 아니라
        step.set()

        # vehicle 제거 때마다 해당 socket thread를 close
        arrived_vehicles = traci.simulation.getArrivedIDList()
        for vid in arrived_vehicles:
            # Ensure that the thread ends
            if vid in threads:
                threads[vid].join()
                del threads[vid]


        # Close SUMO
        if time > 10000:
            break

        #_t.sleep(0.2)

    calcTime_f = _t.time()
    calcTime = calcTime_f - calcTime_s

    traci.close()
    return time, distance, calcTime




# main


for laneSpeed in range(90, 100, 10):
    for routeSpeed in range (6, 7, 1):
        for routeFreq in range (90, 100, 10):

            totalTime = 0
            totalDistance = 0
            totalCalcTime = 0
            for repeat in range (1):
                t, d, ct = simulation(routeSpeed, routeFreq, laneSpeed)
                totalTime += t
                totalDistance += d
                totalCalcTime += ct

            print("routeSpeed :", routeSpeed, "    routeFreq :", routeFreq, "    totalCalcTime :", totalCalcTime, "    totalTime :", totalTime, "    totalDistance :", totalDistance)
            f = open("result.txt", "a")
            #f.write("\nrouteSpeed : " + str(routeSpeed) + "    routeFreq : " + str(routeFreq) + "    totalTime : " + str(totalTime) + "    totalDistance : " + str(int(totalDistance)))
            f.write("\nlaneSpeed : " + str(laneSpeed) + "    totalCalcTime : " + str(totalCalcTime) + "    totalTime : " + str(totalTime) + "    totalDistance : " + str(int(totalDistance)))
            f.close()
