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
		self.quaffle = None

	def __treat_delta_move(self, delta):
		_from, to = tuple(delta['from']), tuple(delta['to'])
		if _from in self.pitch and _from != to:
			moveable = self.pitch.pop(_from)
			moveable.position = to
			self.pitch[to] = moveable

	def __treat_delta_catch(self, delta):
		pos = self.posForID(delta['mid'])
		if pos:
			self.quaffle = self.pitch.pop(pos)

	def __treat_delta_throw(self, delta):
		self.pitch[tuple(delta['from'])] = self.quaffle
		self.quaffle = None

	def posForID(self, pid):
		for pos in self.pitch:
			if self.pitch[pos].mid == pid:
				return pos

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
		for x, y in displacement:
			if not (abs(x) == abs(y) or (x%2 == 0 and y == 0)):
				raise ValueError(str((x, y))+" is not a valid displacement")
		self.client.say(K['MATCH_STROKE'], {
			'mid': player.mid,
			'move': displacement
		})

	def __iter__(self):
		self.waitFor(K['MATCH_MOVEABLES'])
		return self

	def next(self):
		m = self.waitFor(K['MATCH_PROMPT'], K['MATCH_END'])
		if m['type'] == K['MATCH_END']:
			raise StopIteration()
		else:
			return self

	__next__ = next #Py3

