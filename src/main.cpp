#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include <vector>
#include "entities.hpp"

// Helper functions
void gen_asteroids(sf::RenderWindow *window, std::vector<Asteroid> *v, int n);

int main() {
    printf("yes\n"); // yes

    sf::RenderWindow window(sf::VideoMode(800, 600), "Maxsteroid", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);
    sf::Clock clock;
    sf::Event event;
    float timer = 0;

    sf::Font font;
    if (!font.loadFromFile("bebasneue.ttf")) {
        printf("Font bebasneue.ttf is missing...\n");
        return 1;
    }

    sf::Text score_label("", font);
    score_label.setPosition(2, 2);
    score_label.setCharacterSize(12);
    sf::Text level_label("", font);
    level_label.setPosition(2, 16);
    level_label.setCharacterSize(12);

    int score = 50;
    int level = 1;

    Spaceship spaceship = Spaceship(&window, sf::Vector2f(394,294), sf::Vector2f(0,0), 12);
    std::vector<LaserBeam> lasers;
    std::vector<Asteroid> asteroids;
    gen_asteroids(&window, &asteroids, level*2+2);
    
    // Arrow Keys (whether they're pressed)
    // up arrow, down arrow
    int arrows[2] = {0,0};

    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Check the time o_O
        sf::Time delta_time = clock.restart();
        if (asteroids.size() == 0) timer += delta_time.asSeconds();

        // New Level
        if (timer > 5) {
            level += 1;
            timer = 0;
            gen_asteroids(&window, &asteroids, level*2+2);
        }

        // Reset Screen
        window.clear();
        score_label.setString("SCORE " + std::to_string(score));
        window.draw(score_label);
        level_label.setString("LEVEL " + std::to_string(level));
        window.draw(level_label);

        /* Handle Keyboard */
        int precision = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        // Up Arrow (Engine)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            spaceship.engine(100);
            arrows[0]++;
        } else if (arrows[0]) {
            spaceship.engine(0);
            arrows[0] = 0;
        }
        // Down Arrow (Brakes)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            spaceship.engine(-100);
            arrows[1]++;
        } else if (arrows[1]) {
            spaceship.engine(0);
            arrows[1] = 0;
        }
        // Left Arrow (Rotate Anticlockwise)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            spaceship.set_angle(
                spaceship.get_angle() - delta_time.asSeconds()*(precision ? 2 : 10)
            );
        }
        // Right Arrow (Rotate Clockwise)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            spaceship.set_angle(
                spaceship.get_angle() + delta_time.asSeconds()*(precision ? 2 : 10)
            );
        }
        // Space Bar (Shoot)
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && spaceship.shoot() && score) {
            sf::Vector2f coordinates = spaceship.get_coordinates();
            sf::Vector2f point = sf::Vector2f(coordinates.x, coordinates.y+12);
            float angle = spaceship.get_angle();
            lasers.push_back(LaserBeam(
                &window,
                sf::Vector2f(
                    cos(angle)*(point.x-coordinates.x) - sin(angle)*(point.y-coordinates.y) + coordinates.x,
                    cos(angle)*(point.y-coordinates.y) + sin(angle)*(point.x-coordinates.x) + coordinates.y
                ),
                sf::Vector2f(cosf(angle+M_PIl/2)*400, sinf(angle+M_PIl/2)*400),
                angle, 16
            ));

            score -= 1;
        }

        // Render Asteroids
        for (int i = 0; i < asteroids.size(); i++) {
            if (asteroids[i].collision_spaceship(std::vector<Spaceship>{spaceship}) != -1) window.close();
            
            int pos = asteroids[i].collision_laser(lasers);
            if (pos != -1) {
                Asteroid* new_asteroids = asteroids[i].split();
                if (new_asteroids != NULL) {
                    asteroids.push_back(new_asteroids[0]);
                    asteroids.push_back(new_asteroids[1]);
                }
                asteroids.erase(asteroids.begin()+i); 

                lasers.erase(lasers.begin()+pos);

                score += 50;
                continue;
            }
            asteroids[i].physics(delta_time);
            asteroids[i].draw();
        }

        // Render Lasers
        for (int i = 0; i < lasers.size(); i++) {
            if (lasers[i].out_of_bound()) {
                lasers.erase(lasers.begin()+i);
                continue;
            }
            lasers[i].physics(delta_time);
            lasers[i].draw();
        }

        // Render Spaceship
        spaceship.physics(delta_time);
        spaceship.draw();

        window.display();
    }

    // Print Score
    printf("%i\n", score);

    return 0;
}

void gen_asteroids(sf::RenderWindow *window, std::vector<Asteroid> *v, int n) {
    sf::Vector2f point;
    for (int i = 0; i < n; i++) {
        if (randrange(0,1)) point = sf::Vector2f(
            randrange(0,1) ? 82 : (window->getSize().x - 82),
            randrange(82, (int)window->getSize().y-82)
        );
        else point = sf::Vector2f(
            randrange(!2, (int)window->getSize().x-82),
            randrange(0,1) ? 82 : (window->getSize().y - 82)
        );

        v->push_back(Asteroid(
            window,
            point,
            sf::Vector2f(randrange(-40, 40), randrange(-40, 40)),
            randrange(12, 80)
        ));
    }
}