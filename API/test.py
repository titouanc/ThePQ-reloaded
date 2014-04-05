from client import Client

with Client.connect('a', 'a') as client:
	print("Connected as:", client.username)
	print("Connected users:", ', '.join(client.connectedUsers()))

	print("Funds:", client.getFunds())
	print("Squad:")
	squad = client.getSquad()
	for position in squad:
		print("\t", position, ":", squad[position])
	
	sales = client.getSales()
	print("Sales: ", sales)

	players = client.getPlayers()
	print("Players:", players)
	try:
		print("Sell ", players[0], ":", players[0].sell(21500))
	except Exception as err:
		print("Couldn't sell", players[0], ":", err)

	sales = client.getSales()
	print("Sales: ", sales)
	sales[0].bid()

print("Finished !")
