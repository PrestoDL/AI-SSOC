import socket
import threading



def handleClient(c, addr):
    while True:
        try:
            data = c.recv(1024)
            if not data:
                break
        
        except ConnectionResetError:
            break

        print('[' + str(addr[0]) + ':' + str(addr[1]) + ']' + ' > ' + str(data.decode()))
        c.send(('Sent Successfully.').encode())

    c.close()
    print('Disonnected to : ', addr[0], ':', addr[1], sep = '')


def Main():
    host = ""
    port = int(input("Server Port : "))
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.bind((host, port))
    print("Socket binded to port", port)

    s.listen(20000)
    print("Socket is listening")

    threads = []

    while True:
        c, addr = s.accept()
        print('Connected to : ', addr[0], ':', addr[1], sep = '')

        t = threading.Thread(target=handleClient, args=(c, addr))
        threads.append(t)
        t.start()


if __name__ == '__main__':
    Main()
