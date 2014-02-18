import json
from socket import *
from time import sleep
from struct import pack, unpack
from sys import version

HOST, PORT = 'localhost', 32123

class Client:
	def __init__(self, host=HOST, port=PORT):
		self.sock = socket(AF_INET, SOCK_STREAM)
		self.sock.connect((host, port))

	def sendObj(self, obj):
		if version[0] == '2':
			dumped = bytes(json.dumps(obj))
		else:
			dumped = bytes(json.dumps(obj), 'utf-8')
		msglen = len(dumped)
		l = pack('I', htonl(msglen))
		msg = l + dumped
		self.sock.sendall(msg)

	def recvObj(self):
		lbytes = self.sock.recv(4)
		l = ntohl(unpack('I', lbytes)[0])
		dumped = self.sock.recv(l)
		if version[0] == '2':
			return json.loads(str(dumped))
		else:
			return json.loads(str(dumped, 'utf-8'))

if __name__ == '__main__':
	client = Client()

	client.sendObj({'player': 'titou'})
	client.sendObj({'titou': 'an'})
	print(client.recvObj())
	print(client.recvObj())
