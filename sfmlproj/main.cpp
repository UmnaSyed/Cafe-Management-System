#include "cafe.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Cafe Management System");
    Cafe cafe;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            cafe.handleEvent(event, window);
        }

        window.clear(sf::Color(255, 235, 183));
        cafe.draw(window);
        window.display();
    }

    return 0;
}
