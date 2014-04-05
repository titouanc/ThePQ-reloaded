import re

PATTERNS = {
	str : re.compile(r'static +const +std::string +(?P<name>\w[\w\d_]*) *= *"(?P<val>[^"]+)"'),
	int : re.compile(r'static +const(?: +unsigned)? +int +(?P<name>\w[\w\d_]*) *= *(?P<val>\d+)'),
	float : re.compile(r'static +const +(double|float) +(?P<name>\w[\w\d_]*) *= *(?P<val>[\d\.]+)')
}

def parse_constants(filename):
	"""
	Parse a Constant.hpp-like file. All const std::string will be returned
	as a dictionary {name: value}
	"""
	res = {}
	with open(filename) as f:
		for line in f:
			for typ in PATTERNS:
				regexp = PATTERNS[typ]
				match = regexp.search(line)
				if match:
					res[match.group('name')] = typ(match.group('val'))
	return res

K = parse_constants("../common/Constants.hpp")
