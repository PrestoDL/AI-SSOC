import socket
import threading



def eachClient(id, ip, port):
    try:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((ip, port))

        message = f'Hello from client {id}'
        for i in range(1):
            s.send((message).encode())

            data = s.recv(1024)
            print('{' + str(ip) + ':' + str(port) + '}' + ' > ' + str(data.decode()))

        s.close()

    except Exception as e:
        print(f'Client {id} failed to connect : {e}')


def Main():
    ip = input("Connect Address : ")
    port = int(input("Connecting Port : "))
    num_clients = int(input("The Number of Clients : "))

    threads = []

    for i in range(num_clients):
        t = threading.Thread(target=eachClient, args=(i, ip, port))
        threads.append(t)
        t.start()

    for t in threads:
        t.join()
    
    print('All threads are done.')


if __name__ == '__main__':
    Main()
