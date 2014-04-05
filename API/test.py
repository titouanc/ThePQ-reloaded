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

    installations = client.getInstallations()
    for installation in installations : 
        print(installation)
    try:
        print("downgrade", installations[0], ":", installations[0].downgrade())
    except Exception as err:
        print("Could not downgrade", installations[0], ":", err)
        print("Funds:", client.getFunds())
    try:
        print("upgrade", installations[0], ":", installations[0].upgrade())
    except Exception as err:
        print("Could not upgrade", installations[0], ":", err)
        print("Funds:", client.getFunds())

    sales = client.getSales()
    print("Sales: ", sales)
    try:
        sales[0].bid()
    except Exception as err:
        print("Could not bid on", sales[0].player, ":", err)

print("Finished !")
