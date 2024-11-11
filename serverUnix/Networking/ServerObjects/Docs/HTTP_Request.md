```mermaid
    mindmap
  root((HTTP-Request))
    Headers
      Start Line 
       (This header differentiates Request and Response as it holds either the Request or the Status Line.)
        Request Line
         [Signature: 
         Method request-target protocol-version CRLF 
         Processing: 
         Usually split on Whitespaces. 
         Recipients of invalid request-lines should respond with 400.
         Not autocorrection as this might bypass the security filters. 
         A request-method longer than any implemented by the server should result in a 501 "Not Implemented" status-code.
         Similiar, a URI longer than any expected one should result in 414 "URI Too Long".
         Minimum lengths of 8000 octets.]
        Status Line
         [Signature: 
         protocol-version status-code code-description|nullCRLF]
      Header Fields
       (case-insensitive
       field-name:field-value
       Order:
       only good practice counts, control data first)

```

GET / HTTP/1.1 

Host: 127.0.0.1

Connection: keep-alive

Cache-Control: max-age=0

sec-ch-ua: "Not/A)Brand";v="99", "Google Chrome";v="115", "Chromium";v="115"

sec-ch-ua-mobile: ?0

sec-ch-ua-platform: "Windows"

DNT: 1

Upgrade-Insecure-Requests: 1

User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/

115.0.0.0 Safari/537.36

Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;

q=0.8,application/signed-exchange;v=b3;q=0.7

cp-extension-installed: Yes

Sec-Fetch-Site: none

Sec-Fetch-Mode: navigate

Sec-Fetch-User: ?1

Sec-Fetch-Dest: document

Accept-Encoding: gzip, deflate, br

Accept-Language: de-DE,de;q=0.9,en-US;q=0.8,en;q=0.7