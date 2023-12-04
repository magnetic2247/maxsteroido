#include "entities.hpp"

/* Base Entity */
Entity::Entity(sf::Vector2f coordinates, sf::Vector2f velocity) {
    m_acceleration = sf::Vector2f(0,0);
    m_velocity = velocity;
    m_coordinates = coordinates;
}
Entity::Entity(){}

void Entity::physics(sf::Time delta_time, sf::Vector2u surface_size) {
    // Calculate new velocity based on acceleration
    m_velocity.x += m_acceleration.x * delta_time.asSeconds(); // Vx
    m_velocity.y += m_acceleration.y * delta_time.asSeconds(); // Vy

    // Calculate new position based on velocity
    m_coordinates.x += m_velocity.x * delta_time.asSeconds(); // Px
    m_coordinates.y += m_velocity.y * delta_time.asSeconds(); // Py

    // We're stuck in a space loop
    if (m_coordinates.x >= surface_size.x) m_coordinates.x = 1; // x
    if (m_coordinates.x <= 0) m_coordinates.x = surface_size.x-1;
    if (m_coordinates.y >= surface_size.y) m_coordinates.y = 1; // y
    if (m_coordinates.y <= 0) m_coordinates.y = surface_size.y-1;
}

void Entity::draw(sf::RenderWindow *surface) {
    surface->draw(new sf::Vertex(m_coordinates), 1, sf::Points);
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


/* Spaceship */
Spaceship::Spaceship(sf::Vector2f coordinates, sf::Vector2f velocity, int length) : Entity(coordinates, velocity) {
    m_lastshot = 0.2;
    m_length = length;
    m_angle = 0;
}
Spaceship::Spaceship(){}

void Spaceship::physics(sf::Time delta_time, sf::Vector2u surface_size) {
    Entity::physics(delta_time, surface_size);
    m_velocity.x = std::max(-120.0, std::min((double)m_velocity.x, 120.0));
    m_velocity.y = std::max(-120.0, std::min((double)m_velocity.y, 120.0));
    m_lastshot += delta_time.asSeconds();
}

void Spaceship::draw(sf::RenderWindow *surface) {
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

    surface->draw(triangle);
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


/* Laser Beam */
LaserBeam::LaserBeam(sf::Vector2f coordinates, sf::Vector2f velocity, float angle, int length) : Entity(coordinates, velocity) {
    m_angle = angle + M_PIl/4;
    m_length  = length;
}
LaserBeam::LaserBeam(){}

void LaserBeam::draw(sf::RenderWindow *surface) {
    sf::Vertex line[2] = {
        sf::Vertex(sf::Vector2f(
            cos(m_angle)*m_length - sin(m_angle)*m_length + m_coordinates.x,
            cos(m_angle)*m_length + sin(m_angle)*m_length + m_coordinates.y
        )),
        sf::Vertex(m_coordinates)
    };

    surface->draw(line, 2, sf::Lines);
}

int LaserBeam::out_of_bound(sf::Vector2u surface_size) {
    return (
        m_coordinates.x >= surface_size.x-1 ||
        m_coordinates.y >= surface_size.y-1 ||
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


/* Asteroids */
Asteroid::Asteroid(sf::Vector2f coordinates, sf::Vector2f velocity, int radius) : Entity(coordinates, velocity) {
    m_radius = radius;
}
Asteroid::Asteroid(){}

void Asteroid::draw(sf::RenderWindow *surface) {
    sf::CircleShape c = sf::CircleShape(m_radius);
    c.setOrigin(m_radius, m_radius);
    c.setPosition(m_coordinates);
    c.setOutlineThickness(1.f);
    c.setFillColor(sf::Color(0,0,0,0));
    c.setOutlineColor(sf::Color(255, 255, 255));
    surface->draw(c);
}

Asteroid* Asteroid::split() {
    if (m_radius/2 <= 8) return NULL;

    Asteroid* new_asteroids = new Asteroid[2] {
        Asteroid(
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

