#include "game.hpp"

GameManager::GameManager(sf::RenderWindow *surface) {
    m_surface = surface;

    m_spaceship = Spaceship(
        sf::Vector2f(
            m_surface->getSize().x/2 - 6,
            m_surface->getSize().x/2 - 6
        ), 
        sf::Vector2f(0,0), 
        12
    );

    gen_asteroids();
}

void GameManager::frame(sf::Time delta_time) {
    /* Handle Keyboard */
    int precision = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
    // Up Arrow (Engine)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
        m_spaceship.engine(100);
        m_arrows[0]++;
    } else if (m_arrows[0]) {
        m_spaceship.engine(0);
        m_arrows[0] = 0;
    }
    // Down Arrow (Brakes)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
        m_spaceship.engine(-100);
        m_arrows[1]++;
    } else if (m_arrows[1]) {
        m_spaceship.engine(0);
        m_arrows[1] = 0;
    }
    // Left Arrow (Rotate Anticlockwise)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        m_spaceship.set_angle(
            m_spaceship.get_angle() - delta_time.asSeconds()*(precision ? 2 : 10)
        );
    }
    // Right Arrow (Rotate Clockwise)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        m_spaceship.set_angle(
            m_spaceship.get_angle() + delta_time.asSeconds()*(precision ? 2 : 10)
        );
    }
    // Space Bar (Shoot)
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && m_spaceship.shoot() && m_score) {
        sf::Vector2f coordinates = m_spaceship.get_coordinates();
        sf::Vector2f point = sf::Vector2f(coordinates.x, coordinates.y+12);
        float angle = m_spaceship.get_angle();
        m_lasers.push_back(LaserBeam(
            sf::Vector2f(
                cos(angle)*(point.x-coordinates.x) - sin(angle)*(point.y-coordinates.y) + coordinates.x,
                cos(angle)*(point.y-coordinates.y) + sin(angle)*(point.x-coordinates.x) + coordinates.y
            ),
            sf::Vector2f(cosf(angle+M_PIl/2)*400, sinf(angle+M_PIl/2)*400),
            angle, 16
        ));

        m_score -= 1;
    }

    // Render Asteroids
    for (int i = 0; i < m_asteroids.size(); i++) {
        if (m_asteroids[i].collision_spaceship(std::vector<Spaceship>{m_spaceship}) != -1) m_surface->close();
        
        int pos = m_asteroids[i].collision_laser(m_lasers);
        if (pos != -1) {
            Asteroid* new_asteroids = m_asteroids[i].split();
            if (new_asteroids != NULL) {
                m_asteroids.push_back(new_asteroids[0]);
                m_asteroids.push_back(new_asteroids[1]);
            }
            m_asteroids.erase(m_asteroids.begin()+i); 

            m_lasers.erase(m_lasers.begin()+pos);

            m_score += (m_level%2) ? m_level*50 : (m_level-1)*50;
            continue;
        }
        m_asteroids[i].physics(delta_time, m_surface->getSize());
        m_asteroids[i].draw(m_surface);
    }

    // Render Lasers
    for (int i = 0; i < m_lasers.size(); i++) {
        if (m_lasers[i].out_of_bound(m_surface->getSize())) {
            m_lasers.erase(m_lasers.begin()+i);
            continue;
        }
        m_lasers[i].physics(delta_time, m_surface->getSize());
        m_lasers[i].draw(m_surface);
    }

    // Render Spaceship
    m_spaceship.physics(delta_time, m_surface->getSize());
    m_spaceship.draw(m_surface);
}

void GameManager::next_level() {
    m_level++;
    gen_asteroids();
}

int GameManager::asteroids_left() {
    return m_asteroids.size();
}

void GameManager::gen_asteroids() {
    sf::Vector2u surface_size = m_surface->getSize();
    sf::Vector2f coordinates;

    for (int i = 0; i < m_level*2+randrange(2, std::max(m_level, 2)); i++) {
        if (randrange(0,1)) {
            coordinates = sf::Vector2f(
                randrange(0,1) ? 82 : (surface_size.x - 82),
                randrange(82, (int)surface_size.y-82)
            );
        } else {
            coordinates = sf::Vector2f(
                randrange(!2, (int)surface_size.x-82),
                randrange(0,1) ? 82 : (surface_size.y - 82)
            );
        }

        m_asteroids.push_back(Asteroid(
            coordinates,
            sf::Vector2f(randrange(-40, 40), randrange(-40, 40)),
            randrange(12, 80)
        ));
    }
}
