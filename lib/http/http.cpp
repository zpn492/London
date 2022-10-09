#include "http.hpp"

/* ************************************************ */
/* HTTPDecodeRequest */
/* ************************************************ */
HTTPDecodeRequest::HTTPDecodeRequest(std::string request, HTTPResponse &r, Logger &log)
    {
    headers = filehandler::split_string(request, '\n');
    
    std::vector<std::string> method = filehandler::split_string(headers[0], ' ');
    getquery = filehandler::split_string(method[1], '?');
    method[1] = getquery[0];

    filepath = method[1].compare("/") == 0 ? r.indexpath : method[1];
    filepath = r.folderpath + filepath;

    if(setRangeIfAny(log)) 
        httpmethod = r.http206;
    else
        httpmethod = r.http200;

    // If the file does not exist
    // or an object is called
    // 
    if(!filehandler::exists_file(filepath.c_str())) 
        {
        // An object is requested
        

        // No answer available
        filepath = r.filenotfoundpath;
        filepath = r.folderpath + filepath;
        httpmethod = r.http404;
        range = false;
        }     
    // A file was requested
    else
        {
        filetype = filehandler::split_string(filepath, '.');
        contentsize = filehandler::file_size(filepath.c_str());
        partialsize = contentsize+1;

        if(range)
            {
            if(to == 0) to = from + std::min(DEFAULTCHUNKSIZE, contentsize);
            to = 0 + std::min((int)to, contentsize);
            contentsize = to - from + 1;
            content = filehandler::get_file_chunk(filepath.c_str(), from, to);
            }       
        else
            {
            content = filehandler::get_file_contents(filepath.c_str());
            } 
        }
    };

void HTTPDecodeRequest::tsend(Sock &s, HTTPResponse &r)
    {
    tsendHeaders(s, r);
    tsendFile(s);
    };

void HTTPDecodeRequest::tsendHeaders(Sock &s, HTTPResponse &r)
    {
    s.tsend(httpmethod.c_str());
    s.tsend(r.headerContentLength(contentsize).c_str());        
    s.tsend(r.headerConnection().c_str()); 
    s.tsend(r.headerAcceptRanges().c_str());
    if(range) s.tsend(r.headerContentRange(from, to, partialsize).c_str());
    s.tsend(r.headerContentType(filetype[1]).c_str()); 
    s.tsend(r.httpEnd);
    };

void HTTPDecodeRequest::tsendFile(Sock &s)
    {
    s.tsend(content.c_str(), contentsize);
    };

bool HTTPDecodeRequest::setRangeIfAny(Logger &log)
    {
    for(int i = 1; i < headers.size(); i++)
        {
        if(headers[i].find("Range") != std::string::npos)
            {
            std::vector<std::string> tmp1 = filehandler::split_string(headers[i], ' ');
            std::vector<std::string> tmp2 = filehandler::split_string(tmp1[1], '=');
            std::vector<std::string> tmp3 = filehandler::split_string(tmp2[1], '-');
            from = atoi(tmp3[0].c_str());
            to = atoi(tmp3[1].c_str());
            range = true;
            return true;
            }
        }
    range = false;
    return false;
    };

/* ************************************************ */
/* HTTPResponse */
/* ************************************************ */

HTTPResponse::HTTPResponse()
    {
    folderpath = "public_html"; 
    indexpath = "/index.html";
    filenotfoundpath = "/filenotfound.html";
    http200 = "HTTP/1.1 200 OK\r\n";
    http206 = "HTTP/1.1 206 Partial Content\r\n";
    http404 = "HTTP/1.1 404 File Not Found\r\n";
    httpEnd = "\r\n";

    types.insert( std::pair<std::string, std::string>("html", "Content-Type: text/html\r\n") );
    types.insert( std::pair<std::string, std::string>("gif", "Content-Type: image/gif\r\n") );
    types.insert( std::pair<std::string, std::string>("png", "Content-Type: image/png\r\n") );
    types.insert( std::pair<std::string, std::string>("jpg", "Content-Type: image/jpeg\r\n") );
    types.insert( std::pair<std::string, std::string>("css", "Content-Type: text/css\r\n") );
    types.insert( std::pair<std::string, std::string>("mp4", "Content-Type: video/mp4\r\n") );
    types.insert( std::pair<std::string, std::string>("jpeg", "Content-Type: image/jpeg\r\n") );
    types.insert( std::pair<std::string, std::string>("JPG", "Content-Type: image/jpeg\r\n") );
    types.insert( std::pair<std::string, std::string>("pdf", "Content-Type: text/javascript\r\n") );
    types.insert( std::pair<std::string, std::string>("js", "Content-Type: text/javascript\r\n") );
    types.insert( std::pair<std::string, std::string>("min", "Content-Type: image/gif\r\n") );
    types.insert( std::pair<std::string, std::string>("mp3", "Content-Type: audio/mp3\r\n") );
    types.insert( std::pair<std::string, std::string>("ogg", "Content-Type: video/ogg\r\n") );
    };

void HTTPResponse::setFolder(const char folder[])
    {
    folderpath = folder;
    };

std::string HTTPResponse::headerContentLength(int length)
    {
    std::string s = "Content-Length: "+ filehandler::int_to_string(length) +"\r\n";
    return s;
    };

std::string HTTPResponse::headerConnection()
    {
    std::string s = "Connection: close\r\n";
    return s;
    };

std::string HTTPResponse::headerContentType(std::string &type)
    {
    std::string s;

    if(types.find(type) != types.end())
        s = types.find(type)->second;
    else
        s = types.find("html")->second;

    return s;
    };

std::string HTTPResponse::headerAcceptRanges()
    {
    std::string s = "Accept-Ranges: bytes\r\n";
    return s;
    };

std::string HTTPResponse::headerContentRange(size_t from, size_t to, int contentsize)
    {
    std::string s = std::string("Content-Range: bytes ")+filehandler::int_to_string(from)+std::string("-");
    s += filehandler::int_to_string(to)+std::string("/")+filehandler::int_to_string(contentsize)+std::string("\r\n");
    return s;
    };

/* ************************************************ */
/* HTTP */
/* ************************************************ */

void HTTP::start(int port, const char ip[], const char folder[])
    {    
    r.setFolder(folder);
    server.topen(port, ip, 0);
    log.write(FGREEN, "HTTPServer is running");
    
    while(true)
        {
        Sock s = server.taccept(log);
        h.handle(s, r, log);
        }

    log.write(FGREEN, "HTTPServer is ended");
    }

/* ************************************************ */
/* HTTPHandler */
/* ************************************************ */

void HTTPHandler::handle(Sock &s, HTTPResponse &r, Logger &log)
    {    
    char buffer[DEFUALTBUFFERSIZE];
    std::string request;
    int recv;

    recv = s.trecv(buffer, DEFUALTBUFFERSIZE);

    if(recv == -1)
        {
        log.write(FRED, "Nothing was received");
        }
    else
        {
        log.write(0, buffer);

        request = buffer;

        HTTPDecodeRequest dr(request, r, log);

        dr.tsend(s, r);
        }
    s.tshutdown();
    };