from parse_constants  import parse_constants
K = parse_constants("../common/Constants.hpp")

class Installations:

	def __init__(self, client):
		self.client = client
		self.update()

	def update(self):
		self.client.say(K['INSTALLATIONS_LIST'], "")
		self.installations = self.client.waitFor(K['INSTALLATIONS_LIST'])['data']

	def show(self):
		print("Your installations : ")
		for installation in self.installations:
			print("-\033[32m",installation['name'], "\033[0m")
			print("  Level :", installation['level'], "| Upgrade cost :", self.getUpgradeCost(installation['name']))

	def upgrade(self, toUpgrade):
		for i in range(len(self.installations)):
			if self.installations[i]['name'] == toUpgrade:
				self.client.say(K['INSTALLATION_UPGRADE'], i)
				msg = self.client.waitFor(K['INSTALLATION_UPGRADE'])
				if msg['data'] == True:
					self.update()
					print("Upgraded", toUpgrade, "successfully!")
					return True
				else:
					print("Could not upgrade "+toUpgrade+". Check your spelling or your budget.")
					return False

	def downgrade(self, toDowngrade):
		for i in range(len(self.installations)):
			if self.installations[i]['name'] == toDowngrade:
				self.client.say(K['INSTALLATION_DOWNGRADE'], i)
				msg = self.client.waitFor(K['INSTALLATION_DOWNGRADE'])
				if msg['data'] == True:
					self.update()
					print("Downgraded", toDowngrade, "successfully!")
					return True
				else:
					print("Could not downgrade "+toDowngrade+".")
					return False

	def getUpgradeCost(self, name):
		return self.getValueAtLevel(name, self.getLevel(name)+1) - self.getValueAtLevel(name, self.getLevel(name))

	def getValueAtLevel(self, name, level):
		for installation in self.installations:
			if installation['name'] == name:
				if level:
					return installation['baseValue']* (2**(level+1))
				else:
					return 0
		print("Could not find", name)
		return -1

	def getLevel(self, name):
		for installation in self.installations:
			if installation['name'] == name:
				return installation['level']
		print("Could not find", name)
		return -1

