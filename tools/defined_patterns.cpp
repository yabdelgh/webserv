#include <utility>

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
	Single colon(":", 1, 1);
	Single space(" ", 1);
	Single white_space(" \n\r\t\f\v");
	Single not_white_space(" \n\r\t\f\v", MATCH_OUT, 1);

	Group spaced_number;
	spaced_number.append(space)
				 .append(number);

	Group int_array(1, 1);
	int_array.append(number)
			 .append(Group().append(Single(space).set_min(-1))
				 			.append(number));

	Group str_array;
	str_array.append(not_white_space)
			 .append(Group().append(space)
				 			.append(not_white_space));

	Or body_size_units;
	body_size_units.append(Sequence("o", 1, 1))
				   .append(Sequence("O", 1, 1))
				   .append(Sequence("k", 1, 1))
				   .append(Sequence("K", 1, 1))
				   .append(Sequence("m", 1, 1))
				   .append(Sequence("M", 1, 1))
				   .append(Sequence("g", 1, 1))
				   .append(Sequence("G", 1, 1));

	Group body_size_limit;
	body_size_limit.append(number)
		  		   .append(body_size_units.set_max(1).set_min(1));
	
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

	Or request_method(1);
	request_method.append(Sequence("POST", 1, 1))
				   .append(Sequence("GET", 1, 1))
				   .append(Sequence("DELETE", 1, 1));

	Group request_methods;
	request_methods.append(request_method)
				   .append(Group().append(space)
				 				  .append(request_method));

	Or on_off(1, 1);
	on_off.append(Sequence("on",1 ,1))
		  .append(Sequence("off",1 ,1));

	Group http_error_code;
	http_error_code.append(Between(number).set_min(3).set_max(3))
				   .append(space);

	Group key;
		key.append(not_white_space);

	Group context_opening;
		context_opening.append(Single(space).set_max(-1).set_min(-1))
					   .append(Single("{", 1));
	
	Group context_closing;
		context_closing.append(Single(space).set_max(-1).set_min(-1))
					   .append(Single("}", 1));

	patterns.insert(pair<string, Pattern>("number", Pattern().append(number)));
	patterns.insert(pair<string, Pattern>("not_spaces", Pattern().append(any)));
	patterns.insert(pair<string, Pattern>("spaces", Pattern().append(space.set_min(-1))));
	patterns.insert(pair<string, Pattern>("white_spaces", Pattern().append(white_space)));
	patterns.insert(pair<string, Pattern>("colon", Pattern().append(colon)));
	patterns.insert(pair<string, Pattern>(" +", Pattern().append(space)));
	patterns.insert(pair<string, Pattern>("int_array", Pattern().append(int_array)));
	patterns.insert(pair<string, Pattern>("str_array", Pattern().append(str_array)));
	patterns.insert(pair<string, Pattern>("[0-9]+[oOkKmMgG]", Pattern().append(body_size_limit)));
	patterns.insert(pair<string, Pattern>("units", Pattern().append(body_size_units)));
	patterns.insert(pair<string, Pattern>("off|on", Pattern().append(on_off)));
	patterns.insert(pair<string, Pattern>("http_methods", Pattern().append(request_methods)));
	patterns.insert(pair<string, Pattern>("ip", Pattern().append(ip)));
	patterns.insert(pair<string, Pattern>("key", Pattern().append(key)));
	patterns.insert(pair<string, Pattern>(" *{", Pattern().append(context_opening)));
	patterns.insert(pair<string, Pattern>(" *}", Pattern().append(context_closing)));
	patterns.insert(pair<string, Pattern>("\\S", Pattern().append(not_white_space)));
	
	
	
	return patterns;

}