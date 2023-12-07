#include <SFML/Graphics.hpp>

class FrameState {
public:
    FrameState(sf::RenderWindow *s, sf::Font *f, sf::Time dt=sf::seconds(0)) {
        this->surface = s;
        this->font = f;
        this->delta_time = dt;
    }

    sf::RenderWindow* surface;
    sf::Font* font;
    sf::Time delta_time;
};
