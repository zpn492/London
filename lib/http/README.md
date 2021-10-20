# Simple HTTP Server
 
> RFC 2616 \
Hypertext Transfer Protocol -- HTTP/1.1 \
https://tools.ietf.org/html/rfc2616 
 
> Supported status code protocols:
 * 200 OK
 * 206 Partial Content
 * 404 Not Found
 *
> Approach
```c++
HTTP server(80, "127.0.0.1", "data/public_html");
server.start();
```