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
REVERSE_K = {K[v] : v for v in K}

if __name__ == "__main__":
	assert PATTERNS[str].search('static const std::string A = "B";') != None
	assert PATTERNS[int].search('static const unsigned int THE_ANSWER = 42;') != None
	assert PATTERNS[int].search('static const int THE_ANSWER = 42;') != None
	assert PATTERNS[float].search('static const float PI = 3.14;') != None
	assert PATTERNS[float].search('static const double PI = 3.14;') != None
