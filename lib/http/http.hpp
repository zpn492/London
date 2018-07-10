#include "../../SimpleJust.h"

#include "../../lib/sock/sock.h"
#include "../../lib/filehandler/filehandler.h"
#include "../../lib/logger/logger.h"

#ifndef __HTTP__
#define __HTTP__

class HTTPResponse
    {
public:
    HTTPResponse();
    void setFolder(const char folder[]);
    std::string headerContentLength(int length);
    std::string headerConnection();
    std::string headerContentType(std::string &type);
    std::string headerAcceptRanges();
    std::string headerContentRange(std::string &range);

    const char *folderpath;
    const char *indexpath;
    const char *filenotfoundpath;
    const char *http200; 
    const char *http206; 
    const char *http404; 
    const char *httpEnd;
    std::map<string, string> types;
    };

class HTTPHandler
    {
public:
    void handle(Sock &s, HTTPResponse &r);
    };

class HTTP
    {
public:
    void start(int port, const char ip[], const char folder[]);
private:
    HTTPResponse r;
    HTTPHandler h;
    Sock server;
    };

#endif