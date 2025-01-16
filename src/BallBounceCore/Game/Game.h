#pragma once

#include <SFML/Graphics.hpp>
#include "../Network/UDPSocket.h"

class Ball : public sf::CircleShape
{
public:
    Ball(float x, float y, float radius);
    float GetVelocity() const;
    sf::Vector2f& GetDirection();
private:
    sf::Vector2f direction;
    float velocity;
};

class Game
{
public:
    Game(UDPSocket* socket, int width, int height, char id);
    void Loop();
private:
    sf::RenderWindow window;
    Ball m_ball;
    UDPSocket* m_socket;
    char gameId;
    
    int m_Width;
    int m_height;
    float m_ballRadius;
};
