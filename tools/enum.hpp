#ifndef ENUM_HPP
#define ENUM_HPP

enum RequestStatus{
    REQUEST_READY,
    HEADER_READY,
    INCOMPLETE_HEADER,
    INCOMPLETE_BODY,
    BAD_REQUEST
};

enum OutPutType
{
    STREAM,
    INFILE
};


#endif