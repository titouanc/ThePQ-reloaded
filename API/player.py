from parse_constants import K, REVERSE_K

class Player(object):
    ABILITIES = ['strength', 'velocity', 'precision', 'chance']

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
        self.hasQuaffle = attrs.get('Q?', False)
        self.position = tuple(attrs.get('position', [0, 0]))
        self.mid = attrs.get('ID', 0)
        self.attrs = attrs

    def __repr__(self):
        id = self.memberID if self.mid == 0 else self.mid
        return "<#%d %s>"%(id, self.name)

    def upgrade_ability(self, name):
        if name not in self.ABILITIES:
            raise ValueError(name)
        self.client.say(K['UPGRADE_PLAYER_ABILITY'], {
            K['PLAYER_ID']: self.memberID,
            K['ABILITY']  : self.ABILITIES.index(name)
        })
        players_updated = self.client.getPlayers()
        for p in players_updated:
            if p.memberID == self.memberID:
                return p

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

Ball = Player
