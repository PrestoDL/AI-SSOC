import traci
import sumolib
import socket
import threading
import pickle



congestionWeight = 0.5

def weight_optimize(nowRoute, net):
    for route in nowRoute:
        #print(route)
        traci.edge.setEffort(route, traci.edge.getEffort(route, traci.simulation.getTime()) + congestionWeight)
    
    weights = []
    for edge_id in traci.edge.getIDList():
        weights.append(traci.edge.getEffort(edge_id, traci.simulation.getTime()))

    return weights


def handleClient(c, addr, net):
    while True:
        try:
            serialRoute = c.recv(2048)
            if not serialRoute:
                break
        
        except ConnectionResetError:
            break

        nowRoute = pickle.loads(serialRoute)

        newWeights = weight_optimize(nowRoute, net)
        serialWeight = pickle.dumps(newWeights)
        c.send(serialWeight)

    c.close()
    print(addr[0], ':', addr[1], ' Arrived.', sep = '')



def server_main():
    host = ""
    port = int(input("Server Port : "))
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((host, port))
    print("Socket binded to port", port)

    s.listen(2000)
    print("Socket is listening")

    sumo_cmd = ["sumo", "-c", "test.sumocfg", "--junction-taz", "--no-warnings", "--random"]
    traci.start(sumo_cmd)
    net = traci.edge.getIDList()
    
    threads = []

    while True:
        c, addr = s.accept()
        print(addr[0], ':', addr[1], ' Departed.', sep = '')

        t = threading.Thread(target=handleClient, args=(c, addr, net))
        threads.append(t)
        t.start()
    
    traci.close()
    

if __name__ == '__main__':
    server_main()