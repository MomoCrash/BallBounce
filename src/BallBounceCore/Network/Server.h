#pragma once
#include <vector>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

#include "UDPSocket.h"

struct ServerData
{
    std::vector<sf::Vector2f> ballsPosition;
    std::vector<sf::Color> ballsColors;
};

class Server : public UDPSocket
{
private:
    ServerData m_data;
public:
    Server(PCSTR ip, u_short p) ;
    ServerData& GetServerData();
};