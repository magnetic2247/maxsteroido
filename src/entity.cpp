#include "entities.hpp"

Entity::Entity(sf::RenderWindow *screen, sf::Vector2f coordinates, sf::Vector2f velocity) {
    m_screen = screen;
    m_acceleration = sf::Vector2f(0,0);
    m_velocity = velocity;
    m_coordinates = coordinates;
}

void Entity::physics(sf::Time delta_time) {
    // Calculate new velocity based on acceleration
    m_velocity.x += m_acceleration.x * delta_time.asSeconds(); // Vx
    m_velocity.y += m_acceleration.y * delta_time.asSeconds(); // Vy

    // Calculate new position based on velocity
    m_coordinates.x += m_velocity.x * delta_time.asSeconds(); // Px
    m_coordinates.y += m_velocity.y * delta_time.asSeconds(); // Py

    // We're stuck in a space loop
    if (m_coordinates.x >= m_screen->getSize().x) m_coordinates.x = 1; // x
    if (m_coordinates.x <= 0) m_coordinates.x = m_screen->getSize().x-1;
    if (m_coordinates.y >= m_screen->getSize().y) m_coordinates.y = 1; // y
    if (m_coordinates.y <= 0) m_coordinates.y = m_screen->getSize().y-1;
}

void Entity::draw() {
    m_screen->draw(new sf::Vertex(m_coordinates), 1, sf::Points);
}

void Entity::set_acceleration(int new_Ax, int new_Ay) {
    m_acceleration.x = new_Ax;
    m_acceleration.y = new_Ay;
}

void Entity::set_coordinates(int new_Px, int new_Py) {
    m_coordinates.x = new_Px;
    m_coordinates.y = new_Py;
}

void Entity::set_angle(float angle) {
    m_angle = fmod(angle, M_PIl*2);
}

sf::Vector2f Entity::get_acceleration() {
    return m_acceleration;
}

sf::Vector2f Entity::get_coordinates() {
    return m_coordinates;
}

float Entity::get_angle() {
    return m_angle;
}
