#include "entities.hpp"

Spaceship::Spaceship(sf::RenderWindow *screen, sf::Vector2f coordinates, sf::Vector2f velocity, int length) : Entity(screen, coordinates, velocity) {
    m_lastshot = 0.2;
    m_length = length;
    m_angle = 0;
}

void Spaceship::physics(sf::Time delta_time) {
    Entity::physics(delta_time);
    m_velocity.x = std::max(-120.0, std::min((double)m_velocity.x, 120.0));
    m_velocity.y = std::max(-120.0, std::min((double)m_velocity.y, 120.0));
    m_lastshot += delta_time.asSeconds();
}

void Spaceship::draw() {
    sf::ConvexShape triangle;
    triangle.setPointCount(3);
    triangle.setPoint(0, sf::Vector2f(m_length/2, m_length*2));
    triangle.setPoint(1, sf::Vector2f(m_length, 0));
    triangle.setPoint(2, sf::Vector2f(0, 0));

    triangle.setOrigin(m_length/2, m_length);
    triangle.setRotation(m_angle/M_PIl*180);
    triangle.setPosition(m_coordinates);

    triangle.setOutlineThickness(1.f);
    triangle.setFillColor(sf::Color(0,0,0));
    triangle.setOutlineColor(sf::Color(255, 255, 255));

    m_screen->draw(triangle);
}

void Spaceship::engine(float acceleration) {
    m_acceleration = sf::Vector2f(
        cos(m_angle+M_PIl/2)*acceleration,
        sin(m_angle+M_PIl/2)*acceleration
    );
}

int Spaceship::shoot() {
    if (m_lastshot < 0.2) return 0;
    m_lastshot = 0;
    return 1;
}

sf::Rect<int> Spaceship::hitbox() {
    return sf::Rect(
        (int)(m_coordinates.x - m_length/2),
        (int)(m_coordinates.y - m_length/2),
        m_length, m_length*2
    );
}
