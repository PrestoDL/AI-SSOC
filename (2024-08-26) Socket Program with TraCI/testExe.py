import traci
import sumolib



def update_edge_weights(routeSpeed):
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
        '''
        if ((traci.vehicle.getSpeed(vid) == 0) and (traci.vehicle.getWaitingTime(vid) > 70)):
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
        '''
        traci.edge.setEffort(nowEdge, traci.edge.getEffort(nowEdge, traci.simulation.getTime()) + 0.5)

        traci.vehicle.setEffort(vid, nowEdge, 100000)
    #traci.edge.setEffort(nowEdge, min((traci.edge.getEffort(nowEdge, traci.simulation.getTime()) + 0.5 * traci.edge.getWaitingTime(nowEdge)), 20))
        

        



def simulation(routeSpeed, routeFreq):

    # Directory
    sumo_binary = "C:/Program Files (x86)/Eclipse/Sumo/bin/sumo-gui"
    net_dir = "./ql2.complicate.net.xml"
    sumocfg_dir = "./complicate.sumocfg"
    route_dir = "./complicate.rou.xml"
    sumolib.net.readNet(net_dir)

    # Run SUMO
    sumo_cmd = [sumo_binary, "-c", sumocfg_dir, "-r", route_dir, "--junction-taz", "--no-warnings", "--random"]
    traci.start(sumo_cmd)

    # Set variables
    time = 0
    distance = 0

    # Running ...
    while traci.simulation.getMinExpectedNumber() > 0:
        traci.simulationStep()
        time += 1
        '''
        if time // reroute_freq == 0:
            for vid in traci.simulation.getDepartedIDList():
                traci.vehicle.rerouteTraveltime(vid)'''
        
        vehicle_ids = traci.vehicle.getIDList()
        for veh_id in vehicle_ids:
            distance += traci.vehicle.getDistance(veh_id)

        #if time % routeFreq == 0:
            #update_edge_weights(routeSpeed)
            #update_lane_weights()

        # Close SUMO
        if time > 10000:
            break

    traci.close()
    return time, distance




# main



for routeSpeed in range (3, 13, 1):
    for routeFreq in range (10, 310, 10):

        totalTime = 0
        totalDistance = 0
        for repeat in range (10):
            t, d = simulation(routeSpeed, routeFreq)
            totalTime += t
            totalDistance += d

        print("routeSpeed :", routeSpeed, "    routeFreq :", routeFreq, "    totalTime :", totalTime, "    totalDistance :", totalDistance)
        #f = open("result.txt", "a")
        #f.write("\nrouteSpeed : " + str(routeSpeed) + "    routeFreq : " + str(routeFreq) + "    totalTime : " + str(totalTime) + "    totalDistance : " + str(int(totalDistance)))
        #f.close()
