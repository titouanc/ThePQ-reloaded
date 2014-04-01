class Player(object):
    def __init__(self, attrs):
        self.name = attrs.get('name', '')
        self.memberID = attrs.get('memberID', 0)
        self.chance = attrs.get('chance', 0)
        self.precision = attrs.get('precision', 0)
        self.strength = attrs.get('strength', 0)
        self.velocity = attrs.get('velocity', 0)
