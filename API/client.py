"""
Python API for the Pro Quidditch Manager
http://xkcd.com/353/
"""

from parse_constants  import K
from player import Player
from sale import Sale
from installations import *

from socket import *
from struct import pack, unpack
from sys import version
import json

class Client(object):
    DEFAULT_HOST, DEFAULT_PORT = "localhost", 32123

    @classmethod
    def connect(klass, username, password, host=DEFAULT_HOST, port=DEFAULT_PORT):
        res = klass(host, port)
        if res.login(username, password):
            return res

    def __init__(self, host=DEFAULT_HOST, port=DEFAULT_PORT):
        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.connect((host, port))
        self.session = {}

    def __enter__(self):
        return self

    def __exit__(self, exc_type, exc_value, traceback):
        self.sock.close()

    def __send(self, obj):
        # Python2 and 3 have different encoding idioms
        if version[0] == '2':
            dumped = bytes(json.dumps(obj))
        else:
            dumped = bytes(json.dumps(obj), 'utf-8')
        msglen = len(dumped)
        l = pack('I', htonl(msglen))
        msg = l + dumped
        self.sock.sendall(msg)

    def __recv(self):
        lbytes = self.sock.recv(4)
        l = ntohl(unpack('I', lbytes)[0])
        dumped = self.sock.recv(l)
        if version[0] == '2':
            return json.loads(str(dumped))
        else:
            return json.loads(str(dumped, 'utf-8'))

    def waitFor(self, type):
        while True:
            msg = self.__recv()
            if 'type' in msg:
                if msg['type'] == str(type):
                    return msg
                elif msg['type'] == K['TEAM_INFOS']:
                    if 'team' in self.session:
                        self.session['team'].update(msg['data'])
                    else:
                        self.session['team'] = msg['data']

    def say(self, type, data):
        self.__send({"type": str(type), "data": data})

    def __waitTeamInfos(self):
        msg = self.waitFor(K['TEAM_INFOS'])
        self.session['team'] = msg['data']
        return True

    def getPlayers(self):
        self.say(K['PLAYERS_LIST'], {K['USERNAME']: self.session['username']})
        msg = self.waitFor(K['PLAYERS_LIST'])
        return [Player(self, p) for p in msg['data']]

    def connectedUsers(self):
        self.say(K['CONNECTED_USERS_LIST'], "")
        msg = self.waitFor(K['CONNECTED_USERS_LIST'])
        return msg['data']

    def getInstallations(self):
        self.say(K['INSTALLATIONS_LIST'], "")
        msg = self.waitFor(K['INSTALLATIONS_LIST'])
        return [Installation(self, msg['data'][i], i) for i in range(len(msg['data']))]

    def getFunds(self):
        if 'team' in self.session and 'funds' in self.session['team']:
            return self.session['team']['funds']

    def login(self, username, password):
        self.say(K['LOGIN'], {K['USERNAME']: str(username), K['PASSWORD']: str(password)})
        msg = self.waitFor(K['LOGIN'])
        if msg['data'] != K['USER_LOGIN']:
            return False
        self.session['username'] = str(username)
        return self.__waitTeamInfos()

    def logout(self):
        self.say(K['DISCONNECT'], "")
        self.session = {}

    def register(self, username, password, teamname):
        self.say(K['REGISTER'], {K['USERNAME']: str(username), K['PASSWORD']: str(password)})
        msg = self.waitFor(K['REGISTER'])
        if msg['data'] != K['USER_REGISTERED']:
            return False

        # Register team name
        self.say(K['LOGIN'], {K['USERNAME']: str(username), K['PASSWORD']: str(password)})
        msg = self.waitFor(K['LOGIN'])
        if msg['data'] != K['USER_CHOOSE_TEAMNAME']:
            return False

        self.session['username'] = str(username)
        self.say(K['USER_CHOOSE_TEAMNAME'], {K['TEAMNAME']: str(teamname), K['USERNAME']: str(username)})
        return self.__waitTeamInfos()

    def getSquad(self):
        return {
            'chasers': [Player(self, p) for p in self.session['team']['squad']['chasers']],
            'beaters': [Player(self, p) for p in self.session['team']['squad']['beaters']],
            'seeker': Player(self, self.session['team']['squad']['seeker']),
            'keeper': Player(self, self.session['team']['squad']['keeper']),
        }

    def getSales(self):
        self.say(K['PLAYERS_ON_MARKET_LIST'], "")
        msg = self.waitFor(K['PLAYERS_ON_MARKET_LIST'])
        return [Sale(self, p) for p in msg['data']]

    @property
    def username(self):
        return self.session['username']

if __name__ == "__main__":
    from time import time

    c = Client("localhost", 32123)
    print(c.login("a", "a"))
    print(c.session)
    print("Funds:", c.getFunds())
    print(c.connectedUsers())

    print(c.getSquad())
    print(c.getPlayers())

    installations = c.getInstallations()
    for installation in installations : 
        print(installation)
    print("Funds:", c.getFunds())
    installations[0].downgrade()
    print("Funds:", c.getFunds())
    installations[0].upgrade()
    print("Funds:", c.getFunds())

    p = c.getPlayers()[-1]
    #print("Selling", p, " -> ", p.sell(21500))