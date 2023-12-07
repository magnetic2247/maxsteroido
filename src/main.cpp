#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include "game.hpp"

int main() {
    printf("yes\n");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Maxsteroid", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(120);
    sf::err().rdbuf(nullptr);

    sf::Font font;
    if (!font.loadFromFile("assets/bebasneue.ttf")) {
        printf("Font bebasneue.ttf is missing...\n");
        return 1;
    }

    sf::Text score_label("", font);
    score_label.setCharacterSize(12);
    score_label.setPosition(2, 2);
    sf::Text level_label("", font);
    level_label.setCharacterSize(12);
    level_label.setPosition(2, 16);

    sf::Clock clock;
    sf::Event event;
    float timer = 0;

    GameManager game(&window);

    while(window.isOpen()) {
        while(window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();
        }

        // Check the time o_O
        sf::Time delta_time = clock.restart();
        if (!game.asteroids_left()) timer += delta_time.asSeconds();

        // New Level
        if (timer > 5) {
            timer = 0;
            game.next_level();
        }

        // Reset Screen
        window.clear();
        score_label.setString("SCORE " + std::to_string(game.score()));
        window.draw(score_label);
        level_label.setString("LEVEL " + std::to_string(game.level()));
        window.draw(level_label);

        game.frame(delta_time);

        window.display();
    }

    // Print Score
    printf("%i\n", game.score());

    return 0;
}
