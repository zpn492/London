#include "Sock.h"

struct SocketResponse {
	string GetResponse(string position, int value) {
		switch(value) {
			case -1:
			return position + string(": Action failed\n");
			break;
			case 0:
			return position + string(": Connection closed\n");
			break;
			default:
			return position + string(": Success\n");
			break;
		}
	}
} SocketResponse;

bool Sock::topen(int port, const char *ip, int type) {
    if (WSAStartup(MAKEWORD(2, 2), &_wsaData) != 0)
        printf("WSAStartup failed.\n");

    _socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // The sockaddr_in structure specifies the address family,
    // IP address, and port for the socket that is being bound.
    SOCKADDR_IN SockAddr;
    SockAddr.sin_family = AF_INET;
    SockAddr.sin_port = htons(port);
    SockAddr.sin_addr.s_addr = inet_addr(ip);

    // Connect to remote server
    if (type == 1)
    {
        if (connect(_socket, (SOCKADDR*)(&SockAddr), sizeof(SockAddr)) != 0)
            return _connected;
        _connected = TRUE;
    }
    // Listen for incoming connection requests.
    if (type == 0)
    {
        if (bind(_socket,
            (SOCKADDR *)& SockAddr, sizeof(SockAddr)) == -1)
            wprintf(L"bind failed with error: %ld\n", WSAGetLastError());
        if (listen(_socket, 1) == SOCKET_ERROR)
            wprintf(L"listen failed with error: %ld\n", WSAGetLastError());
    }
    return _connected;
};

Sock Sock::taccept(Logger &log) {
    SOCKET client;

    client = accept(_socket, NULL, NULL);

    if(client == INVALID_SOCKET) {
       log.setColor(FRED);
       wprintf(L"Accept failed to with error: %ld\n", WSAGetLastError());
       log.resetColor();
       return Sock();
    } else {
       log.setColor(FYELLOW);
       wprintf(L"Client connected\n");
       log.resetColor();

    }
    return Sock(client);
};

int Sock::tsend(const char *req, int size) {
    if (!_connected)
        return -1;

    int iResult;
    int offset = 0;
    int buflen = size == 0 ? (int)strlen(req) : size;

    while(offset < buflen)
        {
        iResult = send(_socket, req+offset, buflen-offset, 0);
        if(iResult <= 0) break;
        offset += iResult;
        }
    return iResult;
};

int Sock::trecv(char *buffer, int buffersize) {
    if (!_connected)
        return -1;

    int iResult;

    iResult = recv(_socket, buffer, buffersize, 0);

    if (iResult > 0)
    {
        printf("Bytes received: %d\n", iResult);
        iResult = iResult <= buffersize ? iResult : buffersize;
        buffer[iResult] = '\0';
    }
    if(iResult == 0)
        _connected = FALSE;

    printf("%s", SocketResponse.GetResponse(string("Recv"), iResult).c_str());

    return iResult;
};

std::vector<char*> Sock::trecv() {
    char buffer[DEFUALTBUFFERSIZE];
    std::vector<char*> returnBuffer;

    trecv(buffer, DEFUALTBUFFERSIZE);
    returnBuffer.push_back(buffer);

    return returnBuffer;
};

void Sock::tshutdown() {
    shutdown(_socket, SD_SEND);
};

void Sock::tcleanUp() {
    closesocket(_socket);
    WSACleanup();
};