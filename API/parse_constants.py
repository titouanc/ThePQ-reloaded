import re

PATTERN = re.compile(r'static +const +std::string +(\w[\w\d_]+) *= *"([^"]+)"')

def parse_constants(filename):
	"""
	Parse a Constant.hpp-like file. All const std::string will be returned
	as a dictionary {name: value}
	"""
	res = {}
	with open(filename) as f:
		for line in f:
			match = PATTERN.search(line)
			if match:
				res[match.group(1)] = match.group(2)
	return res

K = parse_constants("../common/Constants.hpp")
