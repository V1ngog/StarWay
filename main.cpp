#include <SFML/Graphics.hpp>
#include <windows.h>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;

// Функция для сброса игры
void resetGame(Sprite& ship, 
                std::vector<Sprite>& asteroids,
                Clock& spawnClock,
                Clock& timeSpawnAsteroidClock,
                Clock& timeScore, 
                const Texture& ShipTexture, 
                int& score,
                Text& scoreText) {

    ship.setPosition(400.f, 300.f);
    asteroids.clear();
    spawnClock.restart();
    timeSpawnAsteroidClock.restart();
    timeScore.restart();
    score = 0;

    scoreText.setString("Score: " + std::to_string(score));
    FloatRect textBounds = scoreText.getLocalBounds();
    scoreText.setPosition(800.f - textBounds.width - 10.f, 10.f);
}

int main() {
    setlocale(LC_ALL, "RU");

    int windowWidth = 800, windowHeight = 600, score = 0;
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Sprite test");
    window.setFramerateLimit(60);

    srand(static_cast<unsigned>(time(0)));

    // Загрузка текстур
    Texture ShipTexture;
    if (!ShipTexture.loadFromFile("image/ship.png")) {
        MessageBox(NULL, L"Не удалось загрузить картинку корабля!", L"Ошибка", MB_OK | MB_ICONERROR);
        return -1;
    }

    Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("image/background.png")) {
        MessageBox(NULL, L"Не удалось загрузить картинку фона!", L"Ошибка", MB_OK | MB_ICONERROR);
        return -1;
    }

    Texture asteroidTexture;
    if (!asteroidTexture.loadFromFile("image/asteroid.png")) {
        MessageBox(NULL, L"Не удалось загрузить картинку астероида!", L"Ошибка", MB_OK | MB_ICONERROR);
        return -1;
    }

    Font MyFont;
    if (!MyFont.loadFromFile("font/myfont.ttf")) {
        MessageBox(NULL, L"Не удалось загрузить картинку астероида!", L"Ошибка", MB_OK | MB_ICONERROR);
        return -1;
    }

    Text scoreText;
    scoreText.setFont(MyFont);
    scoreText.setString("Score: " + std::to_string(score));
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(sf::Color::White);

    Sprite background(BackgroundTexture);
    background.setScale(
        windowWidth / static_cast<float>(BackgroundTexture.getSize().x),
        windowHeight / static_cast<float>(BackgroundTexture.getSize().y)
    );

    Sprite ship(ShipTexture);
    ship.setScale(0.2f, 0.2f);
    ship.setOrigin(ShipTexture.getSize().x / 2.f, ShipTexture.getSize().y / 2.f);
    ship.setPosition(400.f, 300.f);

    float speed = 5.f;
    std::vector<Sprite> asteroids;
    Clock spawnClock, timeSpawnAsteroidClock, timeScore;
    float timeSpawnAsteroid = 1.f;
    bool gameOver = false;

    while (window.isOpen()) {
        Event e;
        while (window.pollEvent(e)) {
            if (e.type == Event::Closed)
                window.close();
        }

        if (!gameOver) {
            // Управление кораблём
            if (Keyboard::isKeyPressed(Keyboard::Up)) ship.move(0, -speed);
            if (Keyboard::isKeyPressed(Keyboard::Down)) ship.move(0, speed);
            if (Keyboard::isKeyPressed(Keyboard::Left)) ship.move(-speed, 0);
            if (Keyboard::isKeyPressed(Keyboard::Right)) ship.move(speed, 0);

            // Границы окна
            FloatRect bounds = ship.getGlobalBounds();
            if (bounds.left < 0) ship.setPosition(bounds.width / 2.f, ship.getPosition().y);
            if (bounds.left + bounds.width > windowWidth) ship.setPosition(windowWidth - bounds.width / 2.f, ship.getPosition().y);
            if (bounds.top < 0) ship.setPosition(ship.getPosition().x, bounds.height / 2.f);
            if (bounds.top + bounds.height > windowHeight) ship.setPosition(ship.getPosition().x, windowHeight - bounds.height / 2.f);


            // Спавн астероидов
            if (spawnClock.getElapsedTime().asSeconds() > timeSpawnAsteroid) {
                Sprite asteroid(asteroidTexture);
                float scale = 0.15f + static_cast<float>(rand()) / RAND_MAX * 0.1f;
                asteroid.setScale(scale, scale);
                FloatRect localBounds = asteroid.getLocalBounds();
                asteroid.setOrigin(localBounds.width / 2.f, localBounds.height / 2.f);

                float posY = static_cast<float>(rand() % windowHeight);
                float halfHeight = (localBounds.height * scale) / 2.f;
                if (posY < halfHeight) posY = halfHeight;
                if (posY > windowHeight - halfHeight) posY = windowHeight - halfHeight;

                asteroid.setPosition(windowWidth + (localBounds.width * scale) / 2.f, posY);
                asteroids.push_back(asteroid);
                spawnClock.restart();
            }

            if (timeSpawnAsteroidClock.getElapsedTime().asSeconds() > 10.f) {
                if (timeSpawnAsteroid > 0.3) {
                    timeSpawnAsteroid -= 0.1f;
                    timeSpawnAsteroidClock.restart();
                }
            }

            // Движение астероидов
            for (auto& a : asteroids) a.move(-5.f, 0.f);

            asteroids.erase(
                std::remove_if(asteroids.begin(), asteroids.end(),
                    [](const Sprite& a) { return a.getPosition().x < -200.f; }),
                asteroids.end()
            );

            // Проверка столкновений
            for (auto& a : asteroids) {
                if (ship.getGlobalBounds().intersects(a.getGlobalBounds())) {
                    gameOver = true;
                    break;
                }
            }

            // Получаем размер текста
            FloatRect textBounds = scoreText.getLocalBounds();

            // Ставим позицию в правый верхний угол
            scoreText.setPosition(
                800.f - textBounds.width - 10.f, // окно шириной 800 → отнимаем ширину текста и небольшой отступ
                10.f                             // 10 пикселей от верхнего края
            );

            if (timeScore.getElapsedTime().asSeconds() > 1.f) {
                score += 100;
                scoreText.setString("Score: " + std::to_string(score));
                FloatRect textBounds = scoreText.getLocalBounds();
                scoreText.setPosition(800.f - textBounds.width - 10.f, 10.f);
                timeScore.restart();
            }
        }

        // Рендер
        window.clear();
        window.draw(background);
        window.draw(ship);
        window.draw(scoreText);
        for (auto& a : asteroids) window.draw(a);
        window.display();

        // Если столкновение, показываем окно и сбрасываем игру
        if (gameOver) {
            int result = MessageBox(NULL, L"Вы столкнулись! Начать заново?", L"Game Over", MB_OKCANCEL | MB_ICONEXCLAMATION);
            if (result == IDOK) {
                resetGame(ship,
                        asteroids,
                        spawnClock,
                        timeSpawnAsteroidClock,
                        timeScore,
                        ShipTexture,
                        score,
                        scoreText);

                gameOver = false;
            }
            else {
                window.close();
            }
        }
    }
}
