from parse_constants import K
from player import Player

class Squad(object):
	def __init__(self, client, attrs):
		self.client = client
		self.chasers = [Player(client, p) for p in attrs['chasers']]
		self.beaters = [Player(client, p) for p in attrs['beaters']]
		self.keeper = Player(client, attrs['keeper'])
		self.seeker = Player(client, attrs['seeker'])
		self.owner = attrs.get('squad_owner', '')

	@property
	def players(self):
		return self.chasers + self.beaters + [self.keeper, self.seeker]

	def has_player(self, player):
		return player in self.players
