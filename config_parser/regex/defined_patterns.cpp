#include "./defined_patterns.hpp"

using namespace rgx;
unordered_map<string, Pattern> get_patterns()
{
	unordered_map<string, Pattern> patterns;

	Pattern number;
	number.append(Between('0', '9', 1));
	patterns.insert(pair<string, Pattern>("number", number));

	Pattern ip;
	ip.append(Single(" "))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(" "));
	patterns.insert(pair<string, Pattern>("ip", ip));
	
	return patterns;
}