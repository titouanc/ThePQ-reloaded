from client import Client

PLAYERS = {}

def printPlayers():
	for mid in PLAYERS:
		player = PLAYERS[mid]
		print "\t", mid, player['name'], player['position']

def addPlayers(player_list):
	for player in player_list:
		PLAYERS[player['ID']] = player 

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
			addPlayers(squad['chasers']+squad['beaters']+[squad['keeper'], squad['seeker']])
			print "SQUAD", squad['squad_id']
			printPlayers()
	elif msg['type'] == 'MBALLS':
		addPlayers(msg['data'])
	elif msg['type'] == 'MEND':
		exit()
	elif msg['type'] == 'MDELTA':
		for delta in msg['data']:
			print "%2d"%delta['mid'], PLAYERS[delta['mid']]['name'], delta['from'], '->', delta['to']
			PLAYERS[delta['mid']]['position'] = delta['to']
	elif msg['type'] == 'MSCORES':
		print "======== \033[1m\033[32m WINNER", msg['data']['winner'], "\033[0m"
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

while True:
	# Wait for prompt for both clients
	ok = 0
	while ok < 2: 
		for source in (c, d):
			msg = source.recvObj()
			printMessage(msg)
			if msg['type'] == 'M?':
				ok += 1

	for mid in (1, 4, 7):
		c.sendObj({
			"type": "MSTROKE", 
			"data": {"mid": mid, "move": [[1, 1], [2, 0]]}
		})
		d.sendObj({
			"type": "MSTROKE", 
			"data": {"mid": 10+mid, "move": [[4, 0], [1, 1]]}
		})
		printMessage(c.recvObj())
		printMessage(d.recvObj())
	d.sendObj({
		"type": "MSTROKE", 
		"data": {"mid": 16, "move": [[-1, -1], [3, -3]]}
	})
	printMessage(d.recvObj())

