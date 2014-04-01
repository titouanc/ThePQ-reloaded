from parse_constants  import parse_constants
K = parse_constants("../common/Constants.hpp")

class Installations:

	def __init__(self, client):
		self.client = client
		self.update()

	def update(self):
		self.client.say(K['INSTALLATIONS_LIST'], "")
		self.installations = self.client.waitFor(K['INSTALLATIONS_LIST'])['data']
		print(self.installations)

	def show(self):
		print("Your installations : ")
		for installation in self.installations:
			print("-\033[32m",installation['name'], "\033[0m")
			print("  Level :", installation['level'], "| Upgrade cost : to compute.")

	def upgrade(self, toUpgrade):
		found = False
		for i in range(len(self.installations)):
			if self.installations[i]['name'] == toUpgrade:
				self.client.say(K['INSTALLATION_UPGRADE'], i)
				msg = self.client.waitFor(K['INSTALLATION_UPGRADE'])
				print(msg)
				if msg['data'] == True:
					self.update()
					print("Upgrade successful!")
					return True
				else:
					print("Could not upgrade "+toUpgrade+". Check your spelling or your budget.")
					return False
