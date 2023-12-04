#include "entities.hpp"

class GameManager {
public:
    GameManager(sf::RenderWindow *surface);
    void frame(sf::Time delta_time);
    void next_level();
    int asteroids_left();

    int score() {
        return m_score;
    }
    int level() {
        return m_level;
    }

private:
    void gen_asteroids();

    sf::RenderWindow *m_surface;
    Spaceship m_spaceship;
    std::vector<LaserBeam> m_lasers;
    std::vector<Asteroid> m_asteroids;

    int m_arrows[2] = {0,0};

    int m_score = 50;
    int m_level = 1;
};
