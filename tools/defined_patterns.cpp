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

	Group str_array(1, 1);
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
		  		   .append(Or(body_size_units).set_max(1).set_min(1));
	
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

	Or request_method(1, 1);
	request_method.append(Sequence("POST", 1, 1))
				  .append(Sequence("GET", 1, 1))
				  .append(Sequence("DELETE", 1, 1));

	Group request_methods(1, 1);
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
	
	Or alpha(1);
	alpha.append(Between('a', 'z', 1))
		 .append(Between('A', 'Z', 1));
	
	Group http_header_key(1, 1);
	http_header_key.append(alpha)
				   .append(Group().append(Single("-",1 ,1))
				   				  .append(alpha));

	Or transfer_encoding(1, 1);
	transfer_encoding.append(Sequence("chunked", 1, 1))
					 .append(Sequence("compress", 1, 1))
					 .append(Sequence("deflate", 1, 1))
					 .append(Sequence("gzip", 1, 1));
	
	Or connection;
	connection.append(Sequence("close", 1, 1))
					 .append(Sequence("keep-alive", 1, 1));

	patterns.insert(make_pair("number", Pattern().append(number)));
	patterns.insert(make_pair("not_spaces", Pattern().append(any)));
	patterns.insert(make_pair("spaces", Pattern().append(space.set_min(-1))));
	patterns.insert(make_pair("white_spaces", Pattern().append(white_space)));
	patterns.insert(make_pair("colon", Pattern().append(colon)));
	patterns.insert(make_pair(": ", Pattern().append(Sequence(": ", 1, 1))));
	patterns.insert(make_pair(" +", Pattern().append(space)));
	patterns.insert(make_pair(" ", Pattern().append(space.set_max(1).set_min(1))));
	patterns.insert(make_pair("int_array", Pattern().append(int_array)));
	patterns.insert(make_pair("str_array", Pattern().append(str_array)));
	patterns.insert(make_pair("[0-9]+[oOkKmMgG]", Pattern().append(body_size_limit)));
	patterns.insert(make_pair("units", Pattern().append(body_size_units)));
	patterns.insert(make_pair("off|on", Pattern().append(on_off)));
	patterns.insert(make_pair("http_methods", Pattern().append(request_methods)));
	patterns.insert(make_pair("http_method", Pattern().append(request_method)));
	patterns.insert(make_pair("ip", Pattern().append(ip)));
	patterns.insert(make_pair("key", Pattern().append(key)));
	patterns.insert(make_pair(" *{", Pattern().append(context_opening)));
	patterns.insert(make_pair(" *}", Pattern().append(context_closing)));
	patterns.insert(make_pair("\\S", Pattern().append(not_white_space)));
	patterns.insert(make_pair("\r\n", Pattern().append(Sequence("\r\n", 1, 1)))); // todo: replace \n with \r\n
	patterns.insert(make_pair("[a-zA-Z](-[a-zA-Z])*", Pattern().append(http_header_key)));
	patterns.insert(make_pair("[^\n\r\f\v]+", Pattern().append(Single("\n\r\f\v",MATCH_OUT, 1))));
	patterns.insert(make_pair("transfer_encoding", Pattern().append(transfer_encoding)));
	patterns.insert(make_pair("connection", Pattern().append(connection)));
	
	
	
	return patterns;

}