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

IParseable *get_server_config()
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
                   .push_parseable("unit",Int(p["units"]));
    
    Directive root;
    root.push_parseable(String(p["spaces"]))
         .push_parseable(String(p["not_spaces"]));

    Directive index;
    index.push_parseable(String(p["spaces"]))
         .push_parseable(String(p["str_array"]));

    Directive allowed_methods;
    allowed_methods.push_parseable(String(p["spaces"]));
    allowed_methods.push_parseable(StringArray(p["http_methods"]));

    Directive redirect;
    redirect.push_parseable(String(p["spaces"]));
    redirect.push_parseable(String(p["number"]));
    redirect.push_parseable(String(p["spaces"]));
    redirect.push_parseable(String(p["not_spaces"]));

    Directive autoindex;
    autoindex.push_parseable(String(p["spaces"]));
    autoindex.push_parseable(String(p["off|on"]));
    
    Context location_context(p[" *{"],p[" *}"],p["key"]);
    location_context.insert_parseables("root", root);
    location_context.insert_parseables("allow_methods", allowed_methods);
    location_context.insert_parseables("return", redirect);
    location_context.insert_parseables("index", index);
    location_context.insert_parseables("client_body_buffer_size", body_size_limit);
    location_context.insert_parseables("error_page", Frequent(error_page));

    Directive location;
    location.push_parseable("uri", root);
    location.push_parseable("context", location_context);

    Context server_context(p[" *{"],p[" *}"],p["key"]);
    server_context.insert_parseables("listen", listen);
    server_context.insert_parseables("server_name", server_name);
    server_context.insert_parseables("error_page", error_page);
    server_context.insert_parseables("client_body_buffer_size", body_size_limit);
    server_context.insert_parseables("index", index);
    server_context.insert_parseables("root", root);
    server_context.insert_parseables("return", redirect);
    server_context.insert_parseables("location", location);

    Context server_config(Pattern(), Pattern(), p["key"]);
    server_config.insert_parseables("server", server_context);

    return server_config.clone();
}

