#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <string>
#include "game.hpp"

int main() {
    printf("yes\n");

    sf::RenderWindow window(sf::VideoMode(800, 600), "Maxsteroid", sf::Style::Titlebar | sf::Style::Close);
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

    sf::Text fps("", font);
    fps.setCharacterSize(12);
    fps.setPosition(2, 30);
    float fps_timer = 0;
    int frames = 0;

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

        // FPS Counter
        if (fps_timer >= 1) {
            fps.setString(std::to_string(frames) + "FPS");
            fps_timer = 0;
            frames = 0;
        }
        fps_timer += delta_time.asSeconds();
        frames++;

        // Next Level
        if (timer > 5) {
            timer = 0;
            game.next_level();
        }
        score_label.setString("SCORE " + std::to_string(game.score()));
        level_label.setString("LEVEL " + std::to_string(game.level()));

        // Reset Screen
        window.clear();
        window.draw(fps);
        window.draw(score_label);
        window.draw(level_label);
        game.frame(delta_time);

        window.display();
    }

    // Print Score
    printf("%i\n", game.score());

    return 0;
}
