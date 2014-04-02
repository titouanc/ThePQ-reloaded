from client import Client

with Client.connect('a', 'a') as client:
	print("Connected as:", client.username)
	print("Connected users:", ', '.join(client.connectedUsers()))

	print("Funds:", client.getFunds())
	print("Squad:")
	squad = client.getSquad()
	for position in squad:
		print("\t", position, ":", squad[position])
	
	players = client.getPlayers()
	print("Players:", players)
	print("Sell ", players[0], ":", players[0].sell(21500))

print("Finished !")
