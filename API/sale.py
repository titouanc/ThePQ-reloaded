from parse_constants import K, REVERSE_K
from player import Player
from time import time

class Sale(object):
	class BidError(Exception):
		pass

	def __init__(self, client, attrs):
		self._t0 = time()
		self.client = client
		self.saleID = attrs[K['PLAYER_ID']]
		self.player = Player(client, attrs[K['PLAYER']])
		self.owner = attrs[K['USERNAME']]
		self.bidder = attrs[K['CURRENT_BIDDER']]
		self.turn_nb = attrs[K['TURN_NUMBER']]
		self.time_left = attrs[K['BID_TIMER']]
		self.value = attrs[K['BID_VALUE']]
		self.ended = attrs[K['SALE_STATUS']]

	def __repr__(self):
		return "(Sale %s for %d$ for %ds)"%(
			repr(self.player), 
			self.nextBidValue(), 
			self.remainingTime()
		)

	def nextBidValue(self):
		return self.value*(1+K['BID_RATIO'])

	def remainingTime(self):
		t = self.time_left - (time() - self._t0)
		return max(t, 0)

	def bid(self):
		self.client.say(K['BID_ON_PLAYER_QUERY'], {
			K['USERNAME'] : self.client.username,
			K['PLAYER_ID']: self.player.memberID,
			K['BID_VALUE']: self.nextBidValue()
		})
		msg = self.client.waitFor(K['BID_ON_PLAYER_QUERY'])
		if msg['data'] != K['BID_PLACED']:
			raise self.BidError(REVERSE_K[msg['data']])
		self.owner = self.client.username
		return True

