
import socket
import threading
import socketserver
import time
from random import randint

def timestamp():
    return time.strftime("%H:%M:%S", time.localtime())

def print_timestamp():
    print(timestamp(), end=' ')

class TCPRequestHandler(socketserver.BaseRequestHandler):
    def handle(self):
        print(timestamp(), "Accepting connection from", self.client_address)
        self.data = self.request.recv(1024)
        print(self.data)
        self.reply = bytes([0,11,2])+"Received".encode()
        print(self.reply)
        self.request.send(self.reply)

    def finish(self):
        print(timestamp(), "Closing connection to", self.client_address)

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    def do_the_pingpong(self, a, b):
        pings = randint(a,b)
        print(timestamp(), "Pinging", pings, "times")
        for n in range(pings):
            self.request.send(bytes([0,3,3]))
            print(timestamp(), "Trace> Ping")
            self.data = self.request.recv(16)
            if self.data[2] != 0x04:
                raise ValueError
            print(timestamp(), "Trace> \tPong")

    def handle(self):
        print(timestamp(), "Accepting connection from", self.client_address,
              "in thread", threading.current_thread().name)
        self.request.settimeout(5)
        try:
            self.data = self.request.recv(1024)
            print(timestamp(), "Received", self.data)

            if self.data[2] == 0x01:
                print(timestamp(), "Trace> Message")
                self.reply = bytes([0,5,2,0x4E,0x20])
                print(timestamp(), "Sending", self.reply)
                self.request.send(self.reply)

        except socket.timeout:
            self.request.settimeout(None)
            print(timestamp(), "Trace> Timeout")
            self.do_the_pingpong(3,12)
            self.request.send(bytes([0,3,5]))
            print(timestamp(), "Trace> Terminate")

    def finish(self):
        print(timestamp(), "Closing connection to", self.client_address)

if __name__ == "__main__":
    HOST, PORT, THREAD = "localhost", 20000, True

    try:
        if THREAD:
            server = socketserver.ThreadingTCPServer((HOST,PORT), ThreadedTCPRequestHandler)
            with server:
                server_thread = threading.Thread(target=server.serve_forever)
                server_thread.daemon = True
                server_thread.start()
                print(timestamp(), "Server loop running in thread:", server_thread.name)
                print("Threading server open on ", HOST, ":", PORT, sep='')
                server_thread.join()
        else:
            with socketserver.TCPServer((HOST,PORT), TCPRequestHandler) as server:
                print(timestamp(), "Server open on ", HOST, ":", PORT, sep='')
                server.serve_forever()
    except KeyboardInterrupt:
        print(timestamp(), "Host terminated")