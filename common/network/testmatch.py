from client import Client

c = Client()
d = Client()

#login
d.sendObj({"data": {"D_P": "456", "D_U": "archi"}, "type": "Q_L"})
print d.recvObj()


#login
c.sendObj({"data": {"D_P": "123", "D_U": "titou"}, "type": "Q_L"})
print c.recvObj()

#start match (MAGIC)
c.sendObj({"type": "42"})

# Receive match init sequence
for i in range(3): print c.recvObj()

c.sendObj({
	"type": "MSTROKE", 
	"data": {
		"moveable_id": 13, 
		"move": [[1, 1], [2, 0]]
	}
})
c.sendObj({
	"type": "MSTROKE", 
	"data": {
		"moveable_id": 3, 
		"move": [[1, 1], [2, 0]]
	}
})
while True: print c.recvObj()
