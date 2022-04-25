#include "IParseable.hpp"

#include "simple_types/ASimpleType.hpp"
#include "simple_types/Int.hpp"
#include "simple_types/IntArray.hpp"
#include "simple_types/String.hpp"
#include "simple_types/StringArray.hpp"

#include "complex_types/AComplexType.hpp"
#include "complex_types/Context.hpp"
#include "complex_types/Directive.hpp"
#include "complex_types/Frequent.hpp"

#include "Pattern.hpp"
#include "tools.hpp"

#include "defined_patterns.hpp"

IParseable * get_server_config()
{
    unordered_map<string, Pattern> p = get_patterns();

    Directive listen;
    listen.push_parseable("host", String(p["spaces"]));
    listen.push_parseable(String(p["ip"]));
    listen.push_parseable(String(p["colon"]));
    listen.push_parseable("port", Int(p["number"]));

    Directive server_name;
    server_name.push_parseable(String(p["spaces"]));
    server_name.push_parseable(String(p["not_spaces"]));

    Directive error_page;
    error_page.push_parseable(String(p["spaces"]))
              .push_parseable("codes", IntArray(p["int_array"]))
              .push_parseable(String(p["spaces"]))
              .push_parseable("page", String(p["not_spaces"]));

    Directive body_size_limit;
    body_size_limit.push_parseable(String(p["spaces"]))
                   .push_parseable("limit",Int(p["number"]))
                   .push_parseable("unit",Int(p["unit"]));
    
    Directive index;
    index.push_parseable(String(p["spaces"]))
         .push_parseable(String(p["not_spaces"]));
    Directive root(index);

    Directive allowed_methods;
    allowed_methods.push_parseable(String(p["spaces"]));
    allowed_methods.push_parseable(StringArray(p["http_methods"]));

    Directive redirect;
    redirect.push_parseable(String(p["spaces"]));
    redirect.push_parseable(String(p["number"]));
    redirect.push_parseable(String(p["spaces"]));
    redirect.push_parseable(String(p["not_spaces"]));
    
    Context server();
    server.insert_parseables();
}






