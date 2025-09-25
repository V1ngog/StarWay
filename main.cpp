#include <SFML/Graphics.hpp> // графика
#include <windows.h> // отрисовка окон, MessageBox
#include <vector> // контейнер динамического массива;
#include <cstdlib> // случайные числа, базовые утилиты
#include <ctime> // текущее время(обычно для srand).
using namespace sf;

int main() {
	// Установка русской локали
    setlocale(LC_ALL, "RU");

	// Создание окна
    int windowWidth = 800, windowHeight = 600;
	RenderWindow window(sf::VideoMode(800, 600), "Sprite test"); // размеры окна и заголовок
	window.setFramerateLimit(60); // fps

	srand(static_cast<unsigned>(time(0))); // инициализация генератора случайных чисел


	// Загрузка текстуры корабля
    Texture ShipTexture;
    if (!ShipTexture.loadFromFile("image/ship.png")) {
		MessageBox(NULL, L"Не удалось загрузить картинку!", L"Ошибка", MB_OK | MB_ICONERROR); // Сообщение об ошибке
        return -1;

    }

	// Загрузка текстуры фона
	Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("image/background.png")) {
		MessageBox(NULL, L"Не удалось загрузить картинку!", L"Ошибка", MB_OK | MB_ICONERROR); // Сообщение об ошибке
        return -1;
        Sprite background(BackgroundTexture);
	}

	// Загрузка текстуры астероида
	Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("image/asteroid.png")) {
		MessageBox(NULL, L"Не удалось загрузить картинку!", L"Ошибка", MB_OK | MB_ICONERROR); // Сообщение об ошибке
        return -1;

	}

    // Создание спрайта фона
	Sprite background(BackgroundTexture);
        

	// Создание спрайта корабля
    Sprite ship(ShipTexture);
    ship.setScale(0.2f, 0.2f);
    ship.setOrigin(ShipTexture.getSize().x / 2.f, ShipTexture.getSize().y / 2.f);
    ship.setPosition(400.f, 300.f);

	// Масштабирование фона под размер окна
    background.setScale(
        800.f / BackgroundTexture.getSize().x,
        600.f / BackgroundTexture.getSize().y
    );
	// Скорость перемещения корабля
    float speed = 5.f;

    // Список всех астероидов
    std::vector<Sprite> asteroids;

    // Время для появления новых астероидов
    Clock spawnClock;

	// Главный цикл
    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed)
                window.close();
        }
        // Управление
        if (Keyboard::isKeyPressed(Keyboard::Up))
            ship.move(0, -speed);
        if (Keyboard::isKeyPressed(Keyboard::Down))
            ship.move(0, speed);
        if (Keyboard::isKeyPressed(Keyboard::Left))
            ship.move(-speed, 0);
        if (Keyboard::isKeyPressed(Keyboard::Right))
            ship.move(speed, 0);

        // Границы окна
        FloatRect bounds = ship.getGlobalBounds();

        if (bounds.left < 0)
            ship.setPosition(bounds.width / 2.f, ship.getPosition().y);
        if (bounds.left + bounds.width > windowWidth)
            ship.setPosition(windowWidth - bounds.width / 2.f, ship.getPosition().y);
        if (bounds.top < 0)
		    ship.setPosition(ship.getPosition().x, bounds.height / 2.f);
        if (bounds.top + bounds.height > windowHeight)
			ship.setPosition(ship.getPosition().x, windowHeight - bounds.height / 2.f);
       
		// Появление новых астероидов
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

		// Движение астероидов
        for (auto& a : asteroids) {
            a.move(-5.f, 0.f);
        }

        asteroids.erase(
            std::remove_if(asteroids.begin(), asteroids.end(),
                [](const Sprite& a) { return a.getPosition().x < -200.f; }),
            asteroids.end()
        );


		// Отрисовка
        window.clear(Color::White);
		window.draw(background);
        window.draw(ship);
        for (auto& a : asteroids) {
            window.draw(a);
        }
        window.display();
    }
}
