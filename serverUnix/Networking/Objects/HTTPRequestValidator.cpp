#include "HTTPRequestValidator.hpp"

std::vector<std::string> headers = {"A-IM", "Accept", "Accept-Charset", "Accept-Encoding", "Accept-Language", "Accept-Datetime", "Access-Control-Request-Method", "Access-Control-Request-Headers", "Authorization", "Cache-Control", "Connection", "Content-Length", "Content-Type", "Cookie", "Date", "Expect", "Forwarded", "From", "Host", "If-Match", "If-Modified-Since", "If-None-Match", "If-Range", "If-Unmodified-Since", "Max-Forwards", "Origin", "Pragma", "Proxy-Authorization", "Range", "Referer", "TE", "User-Agent", "Upgrade", "Via", "Warning", "Dnt", "X-Requested-With", "X-CSRF-Token"};

bool validateHeaders(HTTPRequest httprequest)
{
    for (auto const& [field, value] : httprequest.HTTPHeaders)
    {
        auto index = std::find(headers.begin(), headers.end(), field);  

        if (index != headers.end()) {
            continue;
        } else if (field[0] == 'X' && field[1] == '-') {
            // TODO
            // process custom headers
        } else {
            throw std::invalid_argument("detected an invalid header. aborting.");
            return false;
        }
    }
    return true;
}

bool checkHTTPSmuggle(HTTPRequest httprequest)
{
    // TODO
}

bool validateBody(HTTPRequest httprequest)
{
    // TODO
    return true;
}

bool http_request_validator(HTTPRequest httprequest)
{
    if (validateBody(httprequest) && validateHeaders(httprequest)) 
    {
        return true;
    }
    return false;
}