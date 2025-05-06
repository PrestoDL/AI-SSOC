import socket



def Main():
    s = socket.socket(socket.AF_INET,socket.SOCK_STREAM)

    ip = input("Connect Address : ")
    port = int(input("Connecting Port : "))

    s.connect((ip, port))

    while True:
        message = input('[' + str(s.getsockname()[0]) + ':' + str(s.getsockname()[1]) + ']' + ' > ')
        if (message == "exit"):
            break
        s.send((message).encode())

        data = s.recv(1024)
        print('{' + str(ip) + ':' + str(port) + '}' + ' > ' + str(data.decode()))

    s.close()

if __name__ == '__main__':
    Main()
