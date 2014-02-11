import json
from socket import *
from time import sleep

HOST, PORT = 'localhost', 32123

s = socket(AF_INET, SOCK_STREAM)
s.connect((HOST, PORT))

obj = {
	'__type__' : 'TEST',
	'keyA': 'valueA',
	'keyB': {
		'key1': 'val1',
		'key2': 'val2'
	},
	'numbers': list(range(10))
}

dumped = bytes(json.dumps(obj), 'utf-8')
s.sendall(dumped)

print("Sleepin for 5 sec...")
sleep(5)

print(s.recv(100))

s.close()
