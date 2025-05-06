import sumolib
import traci



def simulation():

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
    routeFreq = 30

    # While Simulate
    while traci.simulation.getMinExpectedNumber() > 0:
        traci.simulationStep() # call next step
        time += 1

        if time % routeFreq == 0:
            update_edge_weights() # adjust simulation periodically

        # Close SUMO
        if time > 10000:
            break

    traci.close()
    return time



def update_edge_weights():

    routeSpeed = 5
    
    for vid in traci.vehicle.getIDList(): # 현 시뮬레이션의 모든 차량에 대해 (모든 차량의 식별번호를 참조)

        if traci.vehicle.getSpeed(vid) > routeSpeed : # 차량의 속도를 참조해 상수와 비교
            traci.vehicle.rerouteEffort(vid) # 일정 속도 이상 차량의 경로 재탐색 (느린 차량의 경로 재탐색을 억제)

        nowRoute = traci.vehicle.getRoute(vid) # 차량의 현재 진행경로를 반환
        for route in nowRoute:
                # 예상 진행경로상의 모든 도로에 가중치를 부여 (차량이 특정 경로로 몰리는 것을 방지)
                traci.edge.setEffort(route, traci.edge.getEffort(route, traci.simulation.getTime()) + 0.5)

        nowEdge = traci.vehicle.getRoadID(vid) # 차량의 현재 위치를 반환
        traci.vehicle.setEffort(vid, nowEdge, 100000) # 차량이 왔던 길을 되돌아가지 않도록 설정