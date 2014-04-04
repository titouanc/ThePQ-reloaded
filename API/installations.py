from parse_constants  import parse_constants
K = parse_constants("../common/Constants.hpp")

class Installation:

	def __init__(self, client, attrs, index):
		self.client = client
		self.name = attrs.get('name', '')
		self.baseValue = attrs.get('baseValue', 0)
		self.level = attrs.get('level', 0)
		self.index = index

	def __repr__(self):
		res = "\033[32m" + self.name + "\033[0m"
		res += " | Level\033[33m "+str(self.level)+"\033[0m"
		res += " | Upgrade cost : \033[31m"+str(self.getUpgradeCost())+"\033[0m\n"
		return res

	def upgrade(self):
		self.client.say(K['INSTALLATION_UPGRADE'], self.index)
		msg = self.client.waitFor(K['INSTALLATION_UPGRADE'])
		if msg['data'] == True:
			self.level += 1
			print("Upgraded", self.name, "successfully!")
			return True
		else:
			print("Could not upgrade "+self.name+". Check your spelling or your budget.")
			return False

	def downgrade(self):
		self.client.say(K['INSTALLATION_DOWNGRADE'], self.index)
		msg = self.client.waitFor(K['INSTALLATION_DOWNGRADE'])
		if msg['data'] == True:
			self.level -= 1
			print("Downgraded", self.name, "successfully!")
			return True
		else:
			print("Could not downgrade "+self.name+".")
			return False

	def getUpgradeCost(self):
		return self.getValueAtLevel(self.level+1) - self.getValueAtLevel(self.level)

	def getValueAtLevel(self, level):
		if level:
			return self.baseValue * (2**(level+1))
		return 0
