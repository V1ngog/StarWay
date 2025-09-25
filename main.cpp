#include <SFML/Graphics.hpp>
#include <windows.h> 

using namespace sf;

int main() {
    int windowWidth = 800, windowHeight = 600;
    RenderWindow window(sf::VideoMode(800, 600), "Sprite test");

    Texture ShipTexture;
    if (!ShipTexture.loadFromFile("image/ship.png")) {
        MessageBox(NULL, L"Не удалось загрузить картинку!", L"Ошибка", MB_OK | MB_ICONERROR);
        return -1;
    }
    Sprite ship(ShipTexture);
    ship.setScale(0.1f, 0.2f);
    ship.setOrigin(ShipTexture.getSize().x / 2.f, ShipTexture.getSize().y / 2.f);
    ship.setPosition(400.f, 300.f);

    float speed = 0.1;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::Up))
            ship.move(0, -speed);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            ship.move(0, speed);
        if (Keyboard::isKeyPressed(Keyboard::Left))
            ship.move(-speed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            ship.move(speed, 0);


        FloatRect bounds = ship.getGlobalBounds();

        if (bounds.left < 0)
            ship.setPosition(bounds.width / 2.f, ship.getPosition().y);
       

        window.clear(Color::White);
        window.draw(ship);
        window.display();
    }
}
