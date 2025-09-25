#include <SFML/Graphics.hpp> // �������
#include <windows.h> // ��������� ����, MessageBox
#include <vector> // ��������� ������������� �������;
#include <cstdlib> // ��������� �����, ������� �������
#include <ctime> // ������� �����(������ ��� srand).
using namespace sf;

int main() {
	// ��������� ������� ������
    setlocale(LC_ALL, "RU");

	// �������� ����
    int windowWidth = 800, windowHeight = 600;
	RenderWindow window(sf::VideoMode(800, 600), "Sprite test"); // ������� ���� � ���������
	window.setFramerateLimit(60); // fps

	srand(static_cast<unsigned>(time(0))); // ������������� ���������� ��������� �����


	// �������� �������� �������
    Texture ShipTexture;
    if (!ShipTexture.loadFromFile("image/ship.png")) {
		MessageBox(NULL, L"�� ������� ��������� ��������!", L"������", MB_OK | MB_ICONERROR); // ��������� �� ������
        return -1;

    }

	// �������� �������� ����
	Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("image/background.png")) {
		MessageBox(NULL, L"�� ������� ��������� ��������!", L"������", MB_OK | MB_ICONERROR); // ��������� �� ������
        return -1;
        Sprite background(BackgroundTexture);
	}

	// �������� �������� ���������
	Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("image/asteroid.png")) {
		MessageBox(NULL, L"�� ������� ��������� ��������!", L"������", MB_OK | MB_ICONERROR); // ��������� �� ������
        return -1;

	}

    // �������� ������� ����
	Sprite background(BackgroundTexture);
        

	// �������� ������� �������
    Sprite ship(ShipTexture);
    ship.setScale(0.2f, 0.2f);
    ship.setOrigin(ShipTexture.getSize().x / 2.f, ShipTexture.getSize().y / 2.f);
    ship.setPosition(400.f, 300.f);

	// ��������������� ���� ��� ������ ����
    background.setScale(
        800.f / BackgroundTexture.getSize().x,
        600.f / BackgroundTexture.getSize().y
    );
	// �������� ����������� �������
    float speed = 5.f;

    // ������ ���� ����������
    std::vector<Sprite> asteroids;

    // ����� ��� ��������� ����� ����������
    Clock spawnClock;

	// ������� ����
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        // ����������
        if (Keyboard::isKeyPressed(Keyboard::Up))
            ship.move(0, -speed);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            ship.move(0, speed);
        if (Keyboard::isKeyPressed(Keyboard::Left))
            ship.move(-speed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            ship.move(speed, 0);

        // ������� ����
        FloatRect bounds = ship.getGlobalBounds();

        if (bounds.left < 0)
            ship.setPosition(bounds.width / 2.f, ship.getPosition().y);
        if (bounds.left + bounds.width > windowWidth)
            ship.setPosition(windowWidth - bounds.width / 2.f, ship.getPosition().y);
        if (bounds.top < 0)
		    ship.setPosition(ship.getPosition().x, bounds.height / 2.f);
        if (bounds.top + bounds.height > windowHeight)
			ship.setPosition(ship.getPosition().x, windowHeight - bounds.height / 2.f);
       
		// ��������� ����� ����������
        if (spawnClock.getElapsedTime().asSeconds() > 0.5f) {
            Sprite asteroid(asteroidTexture);

            float scale = 0.2f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
            asteroid.setScale(scale, scale);

            FloatRect bounds = asteroid.getGlobalBounds();

            asteroid.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

            float posY = static_cast<float>(rand() % 600);
            if (posY < bounds.height / 2.f) posY = bounds.height / 2.f;
            if (posY > 600 - bounds.height / 2.f) posY = 600 - bounds.height / 2.f;

            asteroid.setPosition(800.f + bounds.width / 2.f, posY);

            asteroids.push_back(asteroid);
            spawnClock.restart();
        }

		// �������� ����������
        for (auto& a : asteroids) {
            a.move(-5.f, 0.f);
        }

        asteroids.erase(
            std::remove_if(asteroids.begin(), asteroids.end(),
                [](const Sprite& a) { return a.getPosition().x < -200.f; }),
            asteroids.end()
        );


		// ���������
        window.clear(Color::White);
		window.draw(background);
        window.draw(ship);
        for (auto& a : asteroids) {
            window.draw(a);
        }
        window.display();
    }
}
