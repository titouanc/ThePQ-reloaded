"""
A script that always accept friendly match invitations; but does nothing more
"""

import argparse
from sys import argv
from api import Client
from time import sleep

optparser = argparse.ArgumentParser(description="Monitor The Pro Quidditch Market")
optparser.add_argument(
    '-H', '--host', type=str,
    action='store', dest='host', default='localhost',
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
    action='store', dest='username', default="acceptBot",
    help='Username to use (will be created if not registered)'
)
optparser.add_argument(
    '-p', '--password', type=str,
    action='store', dest='password', default="I<3CH1N353F00D",
    help='Password for username'
)
optparser.add_argument(
	'-t', '--team-name', type=str,
	action='store', dest='teamname', default="Accept Bot",
	help='Team name for new accounts'
)

OPTIONS = optparser.parse_args()

if len(argv) > 1:
	server = argv[1]

with Client(OPTIONS.host, OPTIONS.port) as client:
	try:
		client.login(OPTIONS.username, OPTIONS.password)
	except:
		client.register(OPTIONS.username, OPTIONS.password, OPTIONS.teamname)

	while True:
		print("Waiting for invitation")
		while len(client.match_requests) == 0:
			client.waitFor('*')
		other = client.match_requests.pop()
		print("Invite from", other)
		match = client.answer_invitation(other, accept=True)
		for turn in match:
			pass
		print("Match ended")
