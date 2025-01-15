#include "Server.h"

Server::Server(PCSTR ip, u_short p) : UDPSocket(ip, p) {}

ServerData& Server::GetServerData()
{
    return m_data;
}
