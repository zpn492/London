#ifndef __SOCK__
#define __SOCK__

#include "../../SimpleJust.h"

#include "../../lib/logger/logger.h"

using namespace std;

#define DEFUALTBUFFERSIZE 4096	// Remember low buffersize can result in buffer overflow

/**
@auther Benjamin Brandt Ohrt
@version 05-05-2015

This class provides methods to:

1) connect to a remote server/services
2) listen & accept clients
3) Send & Recv bytes

Example 1 - Connect to remote server:

Sock web(80, "127.0.0.1", 1);				// Initialize Web object & Connect to a remote server
const int maxbuffer = 1024;					// Define max buffer size
char buffer1[maxbuffer];					// Intialize a buffer
char buffer2[maxbuffer];					// ...
web.tsend("GET /index.html HTTP/1.1\n\n\");	// Send a GET request
web.trecv(buffer1, maxbuffer);				// Store response in buffer
web.tsend("GET /about.html HTTP/1.1\n\n\");	// ...
web.trecv(buffer2, maxbuffer);				// ...
web.tcleanUp();								// Close connection & Free memory

Example 2 - listen & accept clients:

Sock server(5555, "127.0.0.1", 0);			// Initialize Web object & Listen for new clients
Sock client = server.Accept();				// Accept new client
client.tsend("Welcome\n");					// Send welcome message
cout << client.trecv() << endl;				// Print client response
client.tcleanUp();							// Close connection & Free memory
server.tcleanUp();							// ...

WinSock2 functions is descriped at - 05-05-2015:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms741394(v=vs.85).aspx
*/

class Sock {
public:
    Sock() { _connected = FALSE; }

	Sock(SOCKET sock) { _socket = sock; _connected = TRUE; }

	~Sock() { };

	/**
	Initialize SOCKET & Win Socket API

	Open a connection to a remote server

	@param port to remote server
	@param ip to remote server
	@param type 0 for server & 1 for client
	*/
	bool topen(int port, const char *ip, int type);

	Sock taccept(Logger &log);

	/**
	Send a request/message to the connected remote server

	@param req
	@return -1 upon failure orelse the amount of bytes send
	*/
	int tsend(const char *req, int size=0);

	/**
	Receive a response/message from the connected remote server

	@param buffer to contain server response
	@param buffersize to determine max size of response
	@return -1 upon failure orelse the amount of bytes received
	*/
	int trecv(char *buffer, int buffersize);

	/**
	Receive a response/message from the connected remote server

	Default max buffer of: 4096 bytes

	@return response/message
	*/
	std::vector<char*> trecv();

	void tcleanUp();

	void tshutdown();

	bool isConnected() { return _connected; };

private:
	WSADATA _wsaData;
	SOCKET _socket;
	bool _connected;
};

#endif