#include "Game.h"

#include <iostream>

#include "windows.h"

Ball::Ball(float x, float y, float radius) : sf::CircleShape(radius), direction(10, 10), velocity(std::sqrt(direction.x * direction.x + direction.y * direction.y))
{
    setOutlineThickness(4);
    setOutlineColor(sf::Color::White);
    setFillColor(sf::Color::Green);
    setOrigin(getRadius(), getRadius());
    setPosition(x, y);
}

float Ball::GetVelocity() const
{
    return velocity;
}

sf::Vector2f& Ball::GetDirection()
{
    return direction;
}

Game::Game(UDPSocket* socket, int width, int height, char id)
    : window(sf::VideoMode(width, height), "Bouncing ball"),
      m_ball(width / 2, height / 2, 10.0f),
      m_socket(socket),
      m_Width(width),
      m_height(height),
      m_ballRadius(16.0f),
      gameId(id)
{}

void Game::Loop()
{
    sf::Clock clock;
    sf::Time elapsed = clock.restart();
    const sf::Time update_ms = sf::seconds(1.f / 30.f);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
                window.close();
                break;
                }
        }

        elapsed += clock.restart();
        while (elapsed >= update_ms) {
            const auto pos = m_ball.getPosition();
            const auto delta = update_ms.asSeconds() * m_ball.GetVelocity();
            sf::Vector2f new_pos(pos.x + m_ball.GetDirection().x * delta, pos.y + m_ball.GetDirection().y * delta);

            if (new_pos.x - m_ball.getRadius() < 0) { // left window edge
                m_ball.GetDirection().x *= -1;
                new_pos.x = 0 + m_ball.getRadius();
            } else if (new_pos.x + m_ball.getRadius() >= m_Width) { // right window edge
                m_ball.GetDirection().x *= -1;
                new_pos.x = m_Width - m_ball.getRadius();
            } else if (new_pos.y - m_ball.getRadius() < 0) { // top of window
                m_ball.GetDirection().y *= -1;
                new_pos.y = 0 + m_ball.getRadius();
            } else if (new_pos.y + m_ball.getRadius() >= m_height) { // bottom of window
                m_ball.GetDirection().y *= -1;
                new_pos.y = m_height - m_ball.getRadius();
            }
            m_ball.setPosition(new_pos);
            
            char buf[10];
            int x = m_ball.getPosition().x;
            int y = m_ball.getPosition().y;

            memcpy(buf, &x, 4);
            memcpy(buf+4, &y, 4);
            *(buf+8) = gameId;
            
            m_socket->Send(buf);

            elapsed -= update_ms;
        }

        window.clear(sf::Color(0, 0, 0));
        window.draw(m_ball);
        window.display();
    }
}
