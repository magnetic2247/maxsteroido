#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include <cmath>
#include "random.hpp"

class Entity {
public:
    Entity(sf::Vector2f coordinates, sf::Vector2f velocity);
    Entity();

    void physics(sf::Time delta_time, sf::Vector2u surface_size);
    void draw(sf::RenderWindow *surface);

    void set_acceleration(int new_Ax, int new_Ay);
    void set_coordinates(int new_Px, int new_Py);
    void set_angle(float angle);

    sf::Vector2f get_acceleration();
    sf::Vector2f get_coordinates();
    float get_angle();

protected:
    sf::Vector2f m_acceleration;
    sf::Vector2f m_velocity;
    sf::Vector2f m_coordinates;
    float m_angle;
};

class Spaceship : public Entity {
public:
    Spaceship(sf::Vector2f coordinates, sf::Vector2f velocity, int length);
    Spaceship();

    void physics(sf::Time delta_time, sf::Vector2u surface_size);
    void draw(sf::RenderWindow *surface);
    void engine(float acceleration);
    int shoot();
    sf::Rect<int> hitbox();

protected:
    float m_lastshot;
    int m_length;
};

class LaserBeam : public Entity {
public:
    LaserBeam(sf::Vector2f coordinates, sf::Vector2f velocity, float angle, int length);
    LaserBeam();

    void draw(sf::RenderWindow *surface);
    int out_of_bound(sf::Vector2u surface_size);

    int get_length();
    sf::Vector2f* points();

protected:
    int m_length;
};

class Asteroid : public Entity {
public:
    Asteroid(sf::Vector2f coordinates, sf::Vector2f velocity, int radius);
    Asteroid();
    
    void draw(sf::RenderWindow *surface);
    Asteroid* split();

    // Returns position from first element
    int collision_spaceship(std::vector<Spaceship> spaceships);
    int collision_laser(std::vector<LaserBeam> lasers);

    sf::Rect<int> hitbox();

protected:
    int m_radius;
};
