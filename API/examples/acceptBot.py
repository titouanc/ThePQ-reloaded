from api import Client
from time import sleep

with Client() as client:
	try:
		client.login('Accept Bot', 'I<3CH1N353F00D')
	except:
		client.register('Accept Bot', 'I<3CH1N353F00D', 'Accept Bot')

	while True:
		print("Waiting for invitation")
		while len(client.match_requests) == 0:
			sleep(5)
			client.getPlayers()
		other = client.match_requests.pop()
		print("Invite from", other)
		match = client.answer_invitation(other, accept=True)
		for turn in match:
			print("Match turn")