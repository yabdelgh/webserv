#include "./defined_patterns.hpp"

using namespace rgx;
unordered_map<string, Pattern> get_patterns()
{
	unordered_map<string, Pattern> patterns;

	Between number('0', '9', 1);
	Single any(" ", MATCH_OUT, 1);
	Single space(" ", 1);

	Or body_size_unit;
	body_size_unit.append(Sequence("o"))
				  .append(Sequence("O"))
				  .append(Sequence("k"))
				  .append(Sequence("K"))
				  .append(Sequence("m"))
				  .append(Sequence("M"))
				  .append(Sequence("g"))
				  .append(Sequence("G"));

	Group body_size_limit;
	body_size_limit.append(number)
		  		   .append(body_size_unit.set_max(1).set_min(1));

	Group ip;
	ip.append(Single(" "))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(".", 1, 1))
	  .append(Between('0', '9', 1, 3))
	  .append(Single(" "));

	Or request_methods;
	request_methods.append(Sequence("POST"))
				   .append(Sequence("GET"))
				   .append(Sequence("DELETE"));
	Or on_off;
	on_off.append(Sequence("on"))
		  .append(Sequence("off"));

	Group http_error_code;
	http_error_code.append(number.set_min(3).set_max(3))
				   .append(space);

	patterns.insert(pair<string, Pattern>("number", number));
	patterns.insert(pair<string, Pattern>("ip", ip));
	
	return patterns;
}