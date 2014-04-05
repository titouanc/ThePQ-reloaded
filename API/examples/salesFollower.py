"""
Sales follower: example script that monitors players on market.
Data is refreshed every 5 seconds
"""

import argparse
import os
parentdir = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.sys.path.insert(0,parentdir) 

from time import sleep
from client import Client

USERNAME = 'sales_follower'

optparser = argparse.ArgumentParser(description="Monitor The Pro Quidditch Market")
optparser.add_argument(
    '-H', '--host', type=str,
    action='store', dest='host', default='thepq.partou.se',
    help='The Pro Quidditch Server address'
)
optparser.add_argument(
    '-P', '--port', type=int,
    action='store', dest='port', default=32123,
    help='The Pro Quidditch Server port'
)
optparser.add_argument(
    '-i', '--interval', type=float,
    action='store', dest='interval', default=5,
    help='Refresh interval, in seconds'
)
optparser.add_argument(
    '-u', '--username', type=str,
    action='store', dest='username', default="salesfollower",
    help='Username to use (will be created if not registered)'
)
optparser.add_argument(
    '-p', '--password', type=str,
    action='store', dest='password', default="salesfollower",
    help='Password for username'
)
optparser.add_argument(
	'-t', '--team-name', type=str,
	action='store', dest='teamname', default="salesfollower",
	help='Team name for new accounts'
)

OPTIONS = optparser.parse_args()

with Client(OPTIONS.host, OPTIONS.port) as client:
	try:
		client.login(OPTIONS.username, OPTIONS.password)
	except Client.LoginError as err:
		client.register(OPTIONS.username, OPTIONS.password, OPTIONS.teamname)

	print("Connected to %s:%d"%(OPTIONS.host, OPTIONS.port))

	while True:
		for sale in client.getSales():
			print(sale)
		print('='*80)
		sleep(OPTIONS.interval)