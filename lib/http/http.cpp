#include "http.hpp"

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

std::string HTTPResponse::headerContentRange(std::string &range)
    {
    std::string s = "Content-Range: bytes "+range+"\r\n";
    return s;
    };

/* ************************************************ */
/* HTTP */
/* ************************************************ */

void HTTP::start(int port, const char ip[], const char folder[])
    {    
    Logger log;
    r.setFolder(folder);
    server.topen(port, ip, 0);
    std::cout << "HTTPServer is running" << std::endl;
    
    while(true)
        {
        Sock s = server.taccept(log);
        h.handle(s, r);
        }

    std::cout << "HTTPServer is ended" << std::endl;
    }

/* ************************************************ */
/* HTTPHandler */
/* ************************************************ */

void HTTPHandler::handle(Sock &s, HTTPResponse &r)
    {
    Logger log;
    
    char buffer[DEFUALTBUFFERSIZE];
    int recv, contentsize, type;

    std::string request, filepath, content, tmp;
    std::vector<std::string> tokens, filetype;

    recv = s.trecv(buffer, DEFUALTBUFFERSIZE);

    /* Opdel headers */
    /* ..... */

    /* Find ud af om RANGE indgår blandt headers */
    /* ..... */

    if(recv == -1)
        {
        std::cout << "Nothing was received" << std::endl;
        }
    else
        {
        std::cout << buffer << std::endl;

        request = buffer;

        tokens = filehandler::split_string(request, ' ');

        /* Find GET query */
        /* ..... */

        filepath = tokens[1].compare("/") == 0 ? r.indexpath : tokens[1];
        filepath = r.folderpath + filepath;
        
        if(filehandler::exists_file(filepath.c_str(), log))
            {
            tmp = filepath;
            filetype = filehandler::split_string(tmp, '.');

            content = filehandler::get_file_contents(filepath.c_str(), log);
            contentsize = (int)content.size();
            
            s.tsend(r.http200);
            s.tsend(r.headerContentLength(contentsize).c_str());        
            s.tsend(r.headerConnection().c_str()); 
            s.tsend(r.headerContentType(filetype[1]).c_str()); 
            s.tsend(r.httpEnd);

            std::cout << content.size() << std::endl;
            s.tsend(content.c_str(), contentsize);
            }
        else
            {
            filepath = r.filenotfoundpath;
            filepath = r.folderpath + filepath;
            tmp = filepath;
            filetype = filehandler::split_string(tmp, '.');
            
            filehandler::exists_file(filepath.c_str(), log);
            content = filehandler::get_file_contents(filepath.c_str(), log);
            contentsize = (int)content.size();
            
            s.tsend(r.http404);
            s.tsend(r.headerContentLength(contentsize).c_str());
            s.tsend(r.headerContentType(filetype[1]).c_str());
            s.tsend(r.httpEnd);

            s.tsend(content.c_str());            
            }
        }
    s.tshutdown();
    };