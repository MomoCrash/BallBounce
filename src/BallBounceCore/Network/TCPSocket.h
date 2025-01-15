#pragma once

#ifdef _WIN32
#if _MSC_VER >= 1800
#include <WS2tcpip.h>
#else
#define inet_pton(FAMILY, IP, PTR_STRUCT_SOCKADDR) (*(PTR_STRUCT_SOCKADDR)) = inet_addr((IP))
typedef int socklen_t;
#endif
#include <WinSock2.h>
#ifdef _MSC_VER
#if _WIN32_WINNT >= _WIN32_WINNT_WINBLUE
//!< Win8.1 & higher
#pragma comment(lib, "Ws2_32.lib")
#else
#pragma comment(lib, "wsock32.lib")
#endif
#endif
#else
#include <sys/socket.h>
#include <netinet/in.h> // sockaddr_in, IPPROTO_TCP
#include <arpa/inet.h> // hton*, ntoh*, inet_addr
#include <unistd.h>  // close
#include <cerrno> // errno
#define SOCKET int
#define INVALID_SOCKET ((int)-1)
#endif

#define DEFAULT_BUFFER_LEN 24000
#include <iostream>
#include <string>

class TCPSocket
{
private:
    SOCKET tcpSocket ;
    sockaddr_in socketAdress;
    u_short port;
    PCSTR adress;
public:
    TCPSocket(PCSTR ip, u_short p);
    ~TCPSocket();
    void Connect() const;
    void Receive() const;
    void Send(std::string adress, std::string path, const char* data) const;
    void WriteToFile(std::string adress, std::string path, std::ofstream& stream) const;
    
    void Bind();
    void Listen();
    static std::string GetAddress(const sockaddr_in& addr);
};
