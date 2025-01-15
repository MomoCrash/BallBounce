#include "TCPSocket.h"

#include <fstream>
#include <thread>


TCPSocket::TCPSocket(PCSTR ip, u_short p)
{

    port = p;
    adress = ip;

    tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (tcpSocket == INVALID_SOCKET)
    {
        std::cout << "Erreur creation socket : " << WSAGetLastError() << '\n';
        return;
    }
    
    if (inet_pton(AF_INET, adress, &socketAdress.sin_addr.s_addr) <= 0)
    {
        std::cout << "Bad IP" << '\n';
        return;
    }
    socketAdress.sin_family = AF_INET;
    socketAdress.sin_port = htons(port);
    
    std::cout << "Socket connecte !" << '\n';
}

TCPSocket::~TCPSocket()
{
    closesocket(tcpSocket);
}

void TCPSocket::Connect() const
{
    if (connect(tcpSocket, reinterpret_cast<const sockaddr*>(&socketAdress), sizeof(socketAdress)) == SOCKET_ERROR)
    {
        std::cout << "Erreur connection : " << WSAGetLastError() << '\n';
    }
}

void TCPSocket::Receive() const
{
    int iResult ;

    // Output buffer
    char recvbuf[DEFAULT_BUFFER_LEN];
    int recvbuflen = DEFAULT_BUFFER_LEN;
    int WSAERROR;
    
    // Poll all packets 
    do {
        iResult = recv(tcpSocket, recvbuf, recvbuflen, 0);
        WSAERROR = WSAGetLastError();
        if ( iResult > 0 ) {
            std::cout << "From server: " << recvbuf << '\n';
        }
        else if ( iResult == 0 )
            std::cout << "End" << '\n';
        else if (WSAERROR == WSAETIMEDOUT)
            std::cout << "Error " << WSAERROR << '\n';
    } while( iResult > 0 );
}

void TCPSocket::Send(std::string adress, std::string path, const char* data) const
{
    int iResult ;
    // Request to the server
    std::string sendbuf = "GET " + path + " HTTP/1.1\r\n"
    "Host: " + adress + "\r\n"
    "Connection: close\r\n\r\n"
    "data";
    
    iResult = send( tcpSocket, sendbuf.c_str(), sendbuf.length(), 0 );
    if (iResult == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << '\n';
        closesocket(tcpSocket);
        WSACleanup();
        return;
    }
    std::cout << "Send data" << "\n";
}



void TCPSocket::WriteToFile(std::string adress, std::string path, std::ofstream& stream) const
{
    int iResult ;
    
    // Request to the server
    std::string sendbuf = "GET " + path + " HTTP/1.1\r\n"
    "Host: " + adress + "\r\n"
    "Connection: close\r\n\r\n";
    
    iResult = send( tcpSocket, sendbuf.c_str(), sendbuf.length(), 0 );
    if (iResult == SOCKET_ERROR) {
        std::cout << "Send failed: " << WSAGetLastError() << '\n';
        closesocket(tcpSocket);
        WSACleanup();
        return;
    }
    std::cout << "Send success !" << '\n';

    // Output buffer
    char recvbuf[DEFAULT_BUFFER_LEN];
    int recvbuflen = DEFAULT_BUFFER_LEN;
    int WSAERROR;
    
    // Poll all packets 
    do {
        iResult = recv(tcpSocket, recvbuf, recvbuflen, 0);
        WSAERROR = WSAGetLastError();
        if ( iResult > 0 ) {
            std::cout << "Bytes received: " << iResult << '\n';
            std::cout << "From server: " << recvbuf << '\n';
            stream << recvbuf;
        }
        else if ( iResult == 0 )
            std::cout << "End" << '\n';
        else if (WSAERROR == WSAETIMEDOUT)
            std::cout << "Error " << WSAERROR << '\n';
    } while( iResult > 0 );
    
}

void TCPSocket::Bind()
{
    int res = bind(tcpSocket, (sockaddr*)&socketAdress, sizeof(socketAdress));
    if (res != 0)
    {
        std::cout << "Erreur bind : " << WSAGetLastError();
    }
}

void TCPSocket::Listen()
{
    int res = listen(tcpSocket, SOMAXCONN);
    if (res != 0)
    {
        std::cout << "Erreur listen : " << WSAGetLastError();
    }

    for (;;)
    {
        sockaddr_in from = { 0 };
        socklen_t addrlen = sizeof(from);
        SOCKET newClient = accept(tcpSocket, (SOCKADDR*)(&from), &addrlen);
        if (newClient != INVALID_SOCKET)
        {
            std::thread([newClient, from]() {
                const std::string clientAddress = GetAddress(from);
                const unsigned short clientPort = ntohs(from.sin_port);
                std::cout << "Connexion de " << clientAddress.c_str() << ":" << clientPort << std::endl;
                bool connected = true;
                for(;;)
                {
                    char buffer[200] = { 0 };
                    int ret = recv(newClient, buffer, 199, 0);
                    if (ret == 0 || ret == SOCKET_ERROR)
                        break;
                    std::cout << "[" << clientAddress << ":" << clientPort << "]" << buffer << std::endl;
                    ret = send(newClient, buffer, ret, 0);
                    if (ret == 0 || ret == SOCKET_ERROR)
                        break;
                }
                std::cout << "Deconnexion de [" << clientAddress << ":" << clientPort << "]" << std::endl;
            }).detach();
        }
        else
            break;
    }
}

std::string TCPSocket::GetAddress(const sockaddr_in& addr)
{
    char buff[INET6_ADDRSTRLEN] = { 0 };
    return inet_ntop(addr.sin_family, (void*)&(addr.sin_addr), buff, INET6_ADDRSTRLEN);
}

