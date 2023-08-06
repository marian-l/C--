#include "HTTPRequest.hpp"

using namespace std;

std::unordered_map<std::string, HTTPMethods> methodMap = {
    {"GET", GET},
    {"POST", POST},
    {"PUT", PUT},
    {"HEAD", HEAD},
    {"PATCH", PATCH},
    {"DELETE", DELETE},
    {"CONNECT", CONNECT},
    {"OPTIONS", OPTIONS},
    {"TRACE", TRACE}
};

int process_http_method(string method) 
{
    // UPPERCASE CONVERSION
    method = to_uppercase(method);

    // SEARCH METHOD, RETURN methodMap.end() on failure
    auto it = methodMap.find(method);

    if (it != methodMap.end()) {
        HTTPMethods httpMethod = it->second;
        return httpMethod;
    } else {
        perror("HTTPMethod is not a legal method");
        exit(-1);
    }
};

string process_http_uri(string URI)
{
    return URI;
};

float process_http_version(string HTTPVersion)
{   
    HTTPVersion = HTTPVersion.substr(5);

    float httpversion_float = stof(HTTPVersion);

    return httpversion_float;
}

std::unordered_map<string, string> process_http_headers(string headers) 
{  
    unordered_map<string, string> HTTPHeaders;
    istringstream iss(headers);
    string line;

    while (std::getline(iss, line))
    {
        size_t pos = line.find(": ");
        if (pos != line.npos) 
        {
            string field = line.substr(0, pos);
            string value = line.substr(pos + 2);
            HTTPHeaders.insert_or_assign(field, value);
        }
    }

    return HTTPHeaders;
}

struct HTTPRequest http_request_constructor(char *request_string)
{
    struct HTTPRequest request;
    string body(request_string);
  
    string request_line = tokUntil(body, "\n");
    string headers = tokUntil(body, "\n\n");

    // cut everything but body from processed_string
    body.erase(0, headers.length());

    // cut request_line from headers-string
    headers.erase(0, request_line.length());

    // MARK METHOD
    int pos = request_line.find(" ");
    string method = request_line.substr(0, pos);
    request_line = request_line.erase(0, pos + 1);
    request.Method = process_http_method(method);

    // cout << "request.Method: " << request.Method << endl;
    // cout << "requestline: " << request_line << endl;

    // URI
    pos = request_line.find(" ");
    string URI = request_line.substr(0, pos);
    request_line.erase(0, pos + 1);
    request.URI = process_http_uri(URI);
    
    // cout << "request.URI: " << request.URI << endl;
    // cout << "requestline: " << request_line << endl;

    // HTTPVersion
    string HTTPVersion = request_line;
    request.HTTPVersion = process_http_version(HTTPVersion);

    // cout << request.HTTPVersion << endl;

    // headers
    request.HTTPHeaders = process_http_headers(headers);

    return request;
}

int main() {

    char *request_string = "GET /80 HTTP/1.1\nHost: 172.27.34.56\nConnection: keep-alive\nDNT: 1\nUpgrade-Insecure-Requests: 1\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/115.0.0.0 Safari/537.36\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,/*;q=0.8,application/signed-exchange;v=b3;q=0.7\ncp-extension-installed: Yes\nAccept-Encoding: gzip, deflate\nAccept-Language: de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7\nRange: bytes=85-85\nIf-Range: Wed, 19 Jul 2023 19:15:56 GMT\n\n123";

    struct HTTPRequest request = http_request_constructor(request_string);

    // for (const auto& pair : request.HTTPHeaders) 
    // {
    //     std::cout << pair.first << ": " << pair.second << endl;
    // }

    return 0;
}