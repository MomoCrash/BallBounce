#include "UDPSocket.h"

#include <iostream>
#include <string>

UDPSocket::UDPSocket(PCSTR ip, u_short p)
{

    WSADATA data;
    WSAStartup(MAKEWORD(2, 2), &data);

    port = p;
    adress = ip;

    m_udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (m_udpSocket == SOCKET_ERROR)
    {
        std::cout << "Erreur creation socket " << WSAGetLastError() << "\n";
        return;
    }
    socketAdress.sin_addr.s_addr = INADDR_ANY;
    socketAdress.sin_port = htons(port);
    socketAdress.sin_family = AF_INET;
    
    std::cout << "Socket connecte !" << '\n';
}

UDPSocket::~UDPSocket()
{
    closesocket(m_udpSocket);
    DeleteCriticalSection(&m_section);
    WSACleanup();
}

void UDPSocket::Connect() const
{
    if (bind(m_udpSocket, reinterpret_cast<const sockaddr*>(&socketAdress), sizeof(socketAdress)) != 0)
    {
        std::cout << "Erreur bind socket : " << WSAGetLastError() << "\n";
    }
}

void UDPSocket::OpenSession(u_short portDst)
{

    InitializeCriticalSection(&m_section);
    
    inet_pton(AF_INET, adress, &receiver.sin_addr.s_addr);
    receiver.sin_family = AF_INET;
    receiver.sin_port = htons(portDst);

    std::cout << "Entrez le texte à envoyer (vide pour quitter)> \n";

    DWORD receiveChannel;
    DWORD sendChannel;

    HANDLE thread1 = CreateThread(
       nullptr,
       0,
       ReceiveChannel,
       this,
       0,
       &receiveChannel);

    HANDLE thread2 = CreateThread(
       nullptr,
       0,
       SendChannel,
       this,
       0,
       &sendChannel);
    
}

void UDPSocket::Send(char buf[])
{
    
    int send = sendto(m_udpSocket, buf, sizeof(buf) / sizeof(char), 0, reinterpret_cast<const sockaddr*>(&receiver), sizeof(receiver));
    if (send <= 0)
    {
        std::cout << "Erreur envoi de données : Fermeture du programme.\n";
    }
}

char* UDPSocket::GetSocketBuffer()
{
    return socket_buffer;
}

DWORD WINAPI UDPSocket::ReceiveChannel(LPVOID lpParam)
{
    UDPSocket* socket = static_cast<UDPSocket*>(lpParam);
    std::cout << socket->receiver.sin_port;
    std::string received;
    while (true)
    {
        sockaddr_in from;
        socklen_t fromlen = sizeof(from);
        
        int iResult;
        do
        {
            iResult = recvfrom(socket->m_udpSocket, socket->socket_buffer, 1024, 0, reinterpret_cast<sockaddr*>(&from), &fromlen);
            if (iResult > 0)
            {
                received += socket->socket_buffer;
            }
            else if ( iResult == 0 )
            {
                std::cout << "Message end" << '\n';
            }
            else
            {
                std::cout << "Message receiving failed: " << WSAGetLastError() << '\n';
                break;
            }
            if (!received.empty())
            {
                std::cout << received << "\n";
                received.clear();
            }
        } while (iResult > 0);
    }
}

DWORD WINAPI UDPSocket::SendChannel(LPVOID lpParam)
{
    UDPSocket* socket = static_cast<UDPSocket*>(lpParam);
    std::cin.ignore();
    while (true)
    {
        std::string data;
        std::getline(std::cin, data, '\n');
        
        if (!data.empty())
        {
            std::cout << "Message sent : " << data << std::endl;
            int send = sendto(socket->m_udpSocket, data.c_str(), static_cast<int>(data.length()), 0, reinterpret_cast<const sockaddr*>(&socket->receiver), sizeof(socket->receiver));
            if (send <= 0)
            {
                std::cout << "Erreur envoi de données : Fermeture du programme.\n";
                return 0;
            }
        }
    }
}

