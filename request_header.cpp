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

IParseable *get_request_header()
{
    unordered_map<string, Pattern> p;

    p = get_patterns();

    Directive value;
    value.push_parseable(String(p[": "]));
    value.push_parseable("value", String(p["[^\n\r\f\v]+"]));
    value.push_parseable(String(p["\r\n"]));

    Directive connection;
    connection.push_parseable(String(p[": "]));
    connection.push_parseable("value", String(p["connection"]));
    connection.push_parseable(String(p["\r\n"]));
    
    Directive content_length;
    content_length.push_parseable(String(p[": "]));
    content_length.push_parseable("value", Int(p["number"]));
    content_length.push_parseable(String(p["\r\n"]));

    Directive transfer_encoding;
    transfer_encoding.push_parseable(String(p[": "]));
    transfer_encoding.push_parseable("value", String(p["transfer_encoding"]));
    transfer_encoding.push_parseable(String(p["\r\n"]));


    // Content-Type: text/html; charset=UTF-8

    Context pair(Pattern(), p["\r\n"], p["[a-zA-Z](-[a-zA-Z])*"], Pattern());
    pair.insert_parseables("", value);
    pair.insert_parseables("connection", connection);
    pair.insert_parseables("content-length", content_length);
    pair.insert_parseables("transfer-encoding", transfer_encoding);

    Directive basic_info;
    basic_info.push_parseable("method", String(p["http_method"]));
    basic_info.push_parseable(String(p[" "]));
    basic_info.push_parseable("uri",String(Pattern(p["\\S"])));
    basic_info.push_parseable(String(p[" "]));
    basic_info.push_parseable("version", String(Pattern(p["\\S"])));
    basic_info.push_parseable(String(p["\r\n"]));


    Directive header;
    header.push_parseable(basic_info);
    header.push_parseable(pair);
    
    return header.clone();
}

//   resources https://en.wikipedia.org/wiki/List_of_HTTP_header_fields