#include "entities.hpp"

LaserBeam::LaserBeam(sf::RenderWindow *screen, sf::Vector2f coordinates, sf::Vector2f velocity, float angle, int length) : Entity(screen, coordinates, velocity) {
    m_angle = angle + M_PIl/4;
    m_length  = length;
}

void LaserBeam::draw() {
    sf::Vertex line[2] = {
        sf::Vertex(sf::Vector2f(
            cos(m_angle)*m_length - sin(m_angle)*m_length + m_coordinates.x,
            cos(m_angle)*m_length + sin(m_angle)*m_length + m_coordinates.y
        )),
        sf::Vertex(m_coordinates)
    };

    m_screen->draw(line, 2, sf::Lines);
}

int LaserBeam::out_of_bound() {
    sf::Vector2u screen_size = m_screen->getSize();
    return (
        m_coordinates.x >= screen_size.x-1 ||
        m_coordinates.y >= screen_size.y-1 ||
        m_coordinates.x <= 1 ||
        m_coordinates.y <= 1
    );
}

int LaserBeam::get_length() {
    return m_length;
}

sf::Vector2f* LaserBeam::points() {
    sf::Vector2f center_point = m_coordinates;
    sf::Vector2f* points = new sf::Vector2f[m_length];
    for (int i = 0; i < m_length; i++) {
        sf::Vector2f point = sf::Vector2f(m_coordinates.x, m_coordinates.y+i);
        points[i] = sf::Vector2f(
            cos(m_angle)*(point.x-center_point.x) - sin(m_angle)*(point.y-center_point.y) + m_coordinates.x,
            cos(m_angle)*(point.y-center_point.y) + sin(m_angle)*(point.x-center_point.x) + m_coordinates.y
        );
    }
    return points;
}
