from parse_constants import K
from squad import Squad
from player import Ball

class Match(object):
	WEST  = (-2, 0)
	NWEST = (-1, 1)
	NEAST = ( 1, 1)
	EAST  = ( 2, 0)
	SEAST = ( 1,-1)
	SWEST = (-1,-1)

	def __init__(self, client):
		self.client = client
		self.ended = False
		self.pitch = {}
		self.squad = None

	def __treat_delta_move(self, delta):
		_from, to = tuple(delta['from']), tuple(delta['to'])
		if _from in self.pitch:
			self.pitch[to] = self.pitch[_from]
			self.pitch.pop(_from)
			self.pitch[to].position = to

	def __treat_delta_catch(self, delta):
		pass

	def __treat_delta_throw(self, delta):
		pass

	def read_msg(self):
		message = self.client.waitFor('*')
		if message['type'] == K['MATCH_DELTA']:
			for delta in message['data']:
				if delta['type'] == 1: self.__treat_delta_move(delta)
				elif delta['type'] == 2: self.__treat_delta_catch(delta)
				elif delta['type'] == 3: self.__treat_delta_throw(delta)
		elif message['type'] == K['MATCH_MOVEABLES']:
			for ball in map(lambda x: Ball(self.client, x), message['data']['balls']):
				self.pitch[tuple(ball.position)] = ball
			for squad in map(lambda x: Squad(self.client, x), message['data']['squads']):
				if squad.owner == self.client.username:
					self.squad = squad
				for player in squad.players:
					self.pitch[tuple(player.position)] = player
		return message

	def waitFor(self, *msg_type):
		msg = self.read_msg()
		while msg['type'] not in msg_type:
			msg = self.read_msg()
		return msg

	def move(self, player, displacement):
		pass

	def __iter__(self):
		print(self.client)
		self.waitFor(K['MATCH_MOVEABLES'])
		print(self.pitch, self.squad)
		#self.client.waitFor(K['MATCH_START'])
		return self

	def next(self):
		m = self.waitFor(K['MATCH_PROMPT'], K['MATCH_END'])
		if m['type'] == K['MATCH_END']:
			raise StopIteration()
		else:
			return self

	__next__ = next #Py3

