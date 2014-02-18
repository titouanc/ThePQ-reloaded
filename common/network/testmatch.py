from client import Client

def printMessage(msg):
	if msg['type'] == 'M!!!':
		print "\033[31mERROR  :\033[0m", msg['data']
	elif msg['type'] == 'M?':
		print "\033[1mSend your moves..."
	elif msg['type'] == 'MACK':
		print "\033[32mCONFIRM:\033[0m", msg['data']
	elif msg['type'] == 'MSTART':
		print "\033[1mMatch start !!!\033[0m"
	elif msg['type'] == 'MTOUT':
		print "\033[1mTimeout\033[0m"
	elif msg['type'] == 'MSQUADS':
		for squad in msg['data']:
			print "SQUAD", squad['squad_id']
			for ptype in ('chasers', 'beaters'):
				for player in squad[ptype]:
					print "\t", player['name'], player['position']
			print "\t", squad['seeker']['name'], squad['seeker']['position']
			print "\t", squad['keeper']['name'], squad['keeper']['position']

	else:
		print msg

c = Client()
d = Client()

#login
c.sendObj({"data": {"D_P": "456", "D_U": "archi"}, "type": "Q_L"})
print c.recvObj()


#login
d.sendObj({"data": {"D_P": "123", "D_U": "titou"}, "type": "Q_L"})
print d.recvObj()

#start match (MAGIC)
c.sendObj({"type": "42"})

# Receive match init sequence
for i in range(3): 
	printMessage(c.recvObj())
	printMessage(d.recvObj())

d.sendObj({
	"type": "MSTROKE", 
	"data": {
		"moveable_id": 13, 
		"move": [[1, 1], [2, 0]]
	}
})
c.sendObj({
	"type": "MSTROKE", 
	"data": {
		"moveable_id": 3, 
		"move": [[1, 1], [2, 0]]
	}
})
while True: 
	printMessage(c.recvObj())
	printMessage(d.recvObj())

