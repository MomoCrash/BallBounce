#pragma once
#include <map>
#include <vector>

#include "UDPSocket.h"

struct Vector2
{
    float x, y;
};

struct Color
{
    int r, g, b;
};

struct ServerData
{
    std::map<char, int> clientSessions;
    std::vector<Vector2> ballsPosition;
    std::vector<Color> ballsColors;
};

class Server : public UDPSocket
{
private:
    ServerData m_data;
public:
    Server(PCSTR ip, u_short p) ;
    ServerData& GetServerData();
};