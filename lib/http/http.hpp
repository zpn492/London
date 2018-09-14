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
    std::string headerContentRange(size_t from, size_t to, int contentsize);

    const char *folderpath;
    const char *indexpath;
    const char *filenotfoundpath;
    const char *http200; 
    const char *http206; 
    const char *http404; 
    const char *httpEnd;
    std::map<string, string> types;
    };

class HTTPDecodeRequest
    {
public:
    HTTPDecodeRequest(std::string request, HTTPResponse &r, Logger &log);
    void tsend(Sock &s, HTTPResponse &r);
private:
    void tsendHeaders(Sock &s, HTTPResponse &r);
    void tsendFile(Sock &s);
    bool setRangeIfAny(Logger &log);
    std::vector<std::string> headers;
    std::vector<std::string> getquery;
    std::string httpmethod;
    std::string filepath;
    std::vector<std::string> filetype;
    int contentsize;
    int partialsize;
    std::string content;
    bool range;
    size_t from;
    size_t to;
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