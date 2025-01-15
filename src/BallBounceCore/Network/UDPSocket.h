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
#include <string>

class UDPSocket
{
private:
    SOCKET m_udpSocket;
    sockaddr_in receiver;
    
    sockaddr_in socketAdress;
    u_short port;
    PCSTR adress;
    CRITICAL_SECTION m_section;

    char socket_buffer[1024];

public:

    UDPSocket(PCSTR ip, u_short p);
    ~UDPSocket();
    
    void Connect() const;
    void OpenSession(u_short portDst);
    void Send(char buf[]);

    char* GetSocketBuffer() ;
    
    static DWORD WINAPI ReceiveChannel( LPVOID lpParam );
    static DWORD WINAPI SendChannel ( LPVOID lpParam );
    
};
