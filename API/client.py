from parse_constants  import parse_constants
from socket import *
from struct import pack, unpack
from sys import version
import json

K = parse_constants("../common/Constants.hpp")

class Client(object):
    def __init__(self, host, port):
        self.sock = socket(AF_INET, SOCK_STREAM)
        self.sock.connect((host, port))
        self.session = {}

    def __send(self, obj):
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
            if 'type' in msg and msg['type'] == str(type):
                return msg

    def say(self, type, data):
        self.__send({"type": str(type), "data": data})

    def login(self, username, password):
        self.say(K['LOGIN'], {K['USERNAME']: username, K['PASSWORD']: password})
        done = False
        msg = self.waitFor(K['LOGIN'])
        if msg['data'] != K['USER_LOGIN']:
            return False
        self.session['username'] = username

        msg = self.waitFor(K['TEAM_INFOS'])
        self.session['team'] = msg['data']
        return True

if __name__ == "__main__":
    c = Client("localhost", 32123)
    print(c.login("a", "a"))
    print(c.session)
