from client import Client

PLAYERS = {}

def printPlayers():
	for mid in PLAYERS:
		player = PLAYERS[mid]
		print "\t", mid, player['name'], player['position']

def printMessage(msg):
	if msg['type'] == 'M!!!':
		print "\033[31mERROR  :\033[0m", msg['data']
	elif msg['type'] == 'M?':
		print "\033[1mSend your moves...\033[0m"
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
					PLAYERS[player['ID']] = player
			for player in squad['seeker'], squad['keeper']:
				PLAYERS[player['ID']] = player
			printPlayers()
	elif msg['type'] == 'MEND':
		exit()
	elif msg['type'] == 'MDELTA':
		for delta in msg['data']:
			print delta['mid'], delta['from'], '->', delta['to']
			PLAYERS[delta['mid']]['position'] = delta['to']
		printPlayers()
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

c.sendObj({
	"type": "MSTROKE", 
	"data": {
		"mid": 1, 
		"move": [[1, 1], [2, 0]]
	}
})
d.sendObj({
	"type": "MSTROKE", 
	"data": {
		"mid": 11, 
		"move": [[1, 1], [2, 0]]
	}
})
while True: 
	printMessage(c.recvObj())
	printMessage(d.recvObj())

