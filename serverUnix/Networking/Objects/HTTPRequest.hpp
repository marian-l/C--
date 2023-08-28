#ifndef HTTPRequest_hpp
#define HTTPRequest_hpp

#include <stdio.h>
#include <unordered_map>
#include <string>

#include <iostream>
#include <sstream>
#include "./../../Library/StringFunction.hpp"

#endif
enum HTTPMethods
{
    GET,
    POST,
    PUT,
    HEAD,
    PATCH,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
};

struct HTTPRequest 
{
    int Method;
    std::string URI;
    float HTTPVersion;
    std::unordered_map<std::string, std::string> HTTPHeaders;
    std::string body;
};

extern std::unordered_map<std::string, HTTPMethods> methodMap;

struct HTTPRequest http_request_constructor(char *request_string);


/*
A-IM
Accept 
Accept-Charset 
Accept-Encoding
Accept-Language
Accept-Datetime
Access-Control-Request-Method  
Access-Control-Request-Headers 
Authorization  
Cache-Control  
Connection 
Content-Length 
Content-Type   
Cookie 
Date   
Expect 
Forwarded  
From   
Host   
If-Match   
If-Modified-Since  
If-None-Match  
If-Range   
If-Unmodified-Since
Max-Forwards   
Origin 
Pragma 
Proxy-Authorization
Range  
Referer
TE 
User-Agent 
Upgrade
Via
Warning
Dnt
X-Requested-With   
X-CSRF-Token   
*/
