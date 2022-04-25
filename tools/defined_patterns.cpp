#include <pair>

#include "defined_patterns.hpp"
#include "tokens/AToken.hpp"
#include "tokens/Group.hpp"
#include "tokens/Or.hpp"
#include "tokens/Between.hpp"
#include "tokens/Sequence.hpp"
#include "tokens/Single.hpp"

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

	patterns.insert(pair<string, Pattern>("[0-9]+", Pattern().append(number)));
	patterns.insert(pair<string, Pattern>("[^ ]+", Pattern().append(any)));
	patterns.insert(pair<string, Pattern>(" +", Pattern().append(space)));
	patterns.insert(pair<string, Pattern>(" *", Pattern().append(space.set_min(-1))));
	patterns.insert(pair<string, Pattern>("[0-9]+( +[0-9]+)*", Pattern().append(int_array)));
	patterns.insert(pair<string, Pattern>("[0-9]+[oOkKmMgG]", Pattern().append(body_size_limit)));
	patterns.insert(pair<string, Pattern>("off|on", Pattern().append(on_off)));
	patterns.insert(pair<string, Pattern>("POST|GET|DELETE", Pattern().append(request_methods)));
	patterns.insert(pair<string, Pattern>("[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}", Pattern().append(ip)));

	
	return patterns;

	"[0-9]+"
	"[^ ]+"
	"[0-9]+( +[0-9]+)*"
	"[0-9]+[oOkKmMgG]"
	"off|on"
	"POST|GET|DELETE"
	"[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}"
}