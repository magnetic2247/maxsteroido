#include "entities.hpp"

Asteroid::Asteroid(sf::RenderWindow *screen, sf::Vector2f coordinates, sf::Vector2f velocity, int radius) : Entity(screen, coordinates, velocity) {
    m_radius = radius;
}

void Asteroid::draw() {
    sf::CircleShape c = sf::CircleShape(m_radius);
    c.setOrigin(m_radius, m_radius);
    c.setPosition(m_coordinates);
    c.setOutlineThickness(1.f);
    c.setFillColor(sf::Color(0,0,0,0));
    c.setOutlineColor(sf::Color(255, 255, 255));
    m_screen->draw(c);
}

Asteroid* Asteroid::split() {
    if (m_radius/2 <= 8) return NULL;

    Asteroid* new_asteroids = new Asteroid[2] {
        Asteroid(
            m_screen,
            sf::Vector2f(
                m_coordinates.x+(m_radius/2)*(randrange(0,1)?1:-1)*randrange(1,2),
                m_coordinates.y+(m_radius/2)*(randrange(0,1)?1:-1)*randrange(1,2)
            ),
            sf::Vector2f(
                m_velocity.x + randrange(-20, 20),
                m_velocity.y + randrange(-20, 20)
            ),
            (int) m_radius/2
        ),
        Asteroid(
            m_screen,
            sf::Vector2f(
                m_coordinates.x+m_radius/2*(randrange(0,1)?1:-1)*randrange(1,2),
                m_coordinates.y+m_radius/2*(randrange(0,1)?1:-1)*randrange(1,2)
            ),
            sf::Vector2f(
                m_velocity.x + randrange(-20, 20),
                m_velocity.y + randrange(-20, 20)
            ),
            (int) m_radius/2
        ),
    };
    
    return new_asteroids;
}

int Asteroid::collision_spaceship(std::vector<Spaceship> spaceships) {
    for (int i = 0; i < spaceships.size(); i++) {
        if (this->hitbox().intersects(spaceships[i].hitbox())) return i;
    }
    return -1;
}

int Asteroid::collision_laser(std::vector<LaserBeam> lasers) {
    for (int i = 0; i < lasers.size(); i++) {
        sf::Vector2f *points = lasers[i].points();
        for (int j = 0; j < lasers[i].get_length(); j++) {
            if (this->hitbox().contains(points[j].x, points[j].y)) return i;
        }
    }
    return -1;
}

sf::Rect<int> Asteroid::hitbox() {
    return sf::Rect(
        (int)(m_coordinates.x - m_radius*0.75),
        (int)(m_coordinates.y - m_radius*0.75),
        (int)m_radius*6/4, (int)m_radius*6/4
    );
}
