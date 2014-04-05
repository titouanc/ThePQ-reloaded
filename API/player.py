from parse_constants import K, REVERSE_K

class Player(object):
    class SellException(Exception):
        pass

    def __init__(self, client, attrs):
        self.client = client
        self.name = attrs.get('name', '')
        self.memberID = attrs.get('memberID', 0)
        self.chance = attrs.get('chance', 0)
        self.precision = attrs.get('precision', 0)
        self.strength = attrs.get('strength', 0)
        self.velocity = attrs.get('velocity', 0)
        self.attrs = attrs

    def __repr__(self):
        return "<#%d %s>"%(self.memberID, self.name)

    def sell(self, value):
        self.client.say(K['ADD_PLAYER_ON_MARKET_QUERY'], {
            K['USERNAME']: self.client.username,
            K['PLAYER_ID']: self.memberID,
            K['BID_VALUE']: value
        })
        msg = self.client.waitFor(K['ADD_PLAYER_ON_MARKET_QUERY'])

        if msg['data'] != K['PLAYER_ADDED_ON_MARKET']:
            raise self.SellException(REVERSE_K[msg['data']])
        return True
