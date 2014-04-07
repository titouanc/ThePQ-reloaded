from client import Client

# Context manager: a connected client exists only in the 'with' clause
with Client.connect('a', 'a') as client:
    # User attributes
    print("Connected as:", client.username)
    print("Connected users:", ', '.join(client.connectedUsers()))

    # Team's attribute
    print("Funds:", client.getFunds(), "Action points:", client.getAP())
    print("Squad:")
    squad = client.getSquad()
    for position in squad:
        print("\t", position, ":", squad[position])
    
    # List all players on market
    sales = client.getSales()
    print("Sales: ", sales)

    # List all user's players
    players = client.getPlayers()
    print("Players:", players)

    # Upgrade an ability
    p = players[0]
    print(p, 'velocity:', p.velocity, 'upgraded:', p.upgrade_ability('velocity').velocity)

    # Sell a player
    try:
        print("Sell ", p, ":", p.sell(21500))
    except Exception as err:
        print("Couldn't sell", p, ":", err)

    # Installations attributes
    installations = client.getInstallations()
    for installation in installations : 
        print(installation)

    #Installation upgrade/downgrade
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

    # Bid on market
    sales = client.getSales()
    print("Sales: ", sales)
    try:
        sales[0].bid()
    except Exception as err:
        print("Could not bid on", sales[0].player, ":", err)

print("Finished ! The client disconnected")
with Client() as client:
    print("And reconnected, but without automatic log in")
