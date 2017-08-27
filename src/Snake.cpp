//
// Created by Yann Holme Nielsen on 27.08.17.
//

#include <iostream>
#include "Snake.h"
#include "random.h"


/// The main constructor
/// \param width
/// \param height
Snake::Snake(unsigned width, unsigned height) :
        window({width, height}, "Snake") {

    auto rand = Random<>();
    window.setFramerateLimit(10);

    snk.parts.emplace_back(100, 20, 0);
    snk.parts.emplace_back(80, 20, 1);
    snk.parts.emplace_back(60, 20, 2);
    snk.parts.emplace_back(40, 20, 3);
    snk.parts.emplace_back(20, 20, 4);
    xOffset = 20;
    gitter.columns = window.getSize().x / 20;
    gitter.rows = window.getSize().y / 20;
    spawnFood(rand.getIntInRange(3, gitter.columns - 3), rand.getIntInRange(3, gitter.rows - 3));

    m_font.loadFromFile("/Users/liz3/CLionProjects/Conway-s-Game-Of-Life/font/arial.ttf");
    player_text.setFont(m_font);
    player_text.setFillColor(sf::Color::White);
    player_text.setOutlineColor(sf::Color::Black);
    player_text.setOutlineThickness(3);
    player_text.setCharacterSize(20);
    player_text.setPosition(5, 5);
    run();
}

void Snake::run() {

    while (window.isOpen()) {
        window.clear();
        m_objects.clear();

        checkInput();
        checkFood();
        moveSnake();
        checkCollision();
        setString();
        if (!food.visible) {
            auto rand = Random<>();
            spawnFood(rand.getIntInRange(3, gitter.columns - 3), rand.getIntInRange(3, gitter.rows - 3));

        } else {
            drawRectangle(food.x, food.y, 20, 20);
        }
        window.draw(m_objects.data(), m_objects.size(), sf::Quads);
        window.draw(player_text);
        window.display();
    }
}

void Snake::checkInput() {

    checkCollision();
    sf::Event event{};
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
    if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
        xOffset = 0;
        yOffset = 20;
        changed = true;
    }
    if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        xOffset = 20;
        yOffset = 0;
        changed = true;
    }
    if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        xOffset = -20;
        yOffset = 0;
        changed = true;
    }
    if (!changed && sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        xOffset = 0;
        yOffset = -20;
        changed = true;
    }

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) &&
        !sf::Keyboard::isKeyPressed(sf::Keyboard::S)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::D)
        && !sf::Keyboard::isKeyPressed(sf::Keyboard::A) && changed) {
        changed = false;
    }
}

void Snake::drawRectangle(float x, float y, float width, float height) {

    m_objects.push_back(sf::Vertex({x, y}, sf::Color::White));
    m_objects.push_back(sf::Vertex({x + width, y}, sf::Color::White));
    m_objects.push_back(sf::Vertex({x + width, y + height}, sf::Color::White));
    m_objects.push_back(sf::Vertex({x, y + height}, sf::Color::White));
}

void Snake::moveSnake() {

    auto &first = snk.parts.front();
    first.oldX = first.x;
    first.oldY = first.y;
    first.x = first.x + xOffset;
    first.y = first.y + yOffset;

    drawRectangle(first.x, first.y, 20, 20);
    for (unsigned long i = 1; i != snk.parts.size() - 1; i++) {
        auto &current = snk.parts[i];
        current.oldX = current.x;
        current.oldY = current.y;
        current.x = snk.parts[i - 1].oldX;
        current.y = snk.parts[i - 1].oldY;
        drawRectangle(current.x, current.y, 20, 20);
    }
    if (first.x > window.getSize().x)first.x = 0;

    if (first.y > window.getSize().y)first.y = 0;

    if (first.x < 0)first.x = window.getSize().x + 20;

    if (first.y < 0)first.y = window.getSize().y + 20;


}

void Snake::checkCollision() {
    for (auto &part : snk.parts) {
        for (auto &check : snk.parts) {
            if (check.index != part.index) {
                if (check.x == part.x && check.y == part.y) {
                    int64_t x = snk.parts.front().x;
                    int64_t y = snk.parts.front().y;
                    snk.parts.clear();
                    count = 0;
                    snk.parts.emplace_back(x, y, 0);
                    snk.parts.emplace_back(80, 20, 1);
                    snk.parts.emplace_back(60, 20, 2);
                    snk.parts.emplace_back(40, 20, 3);
                    snk.parts.emplace_back(20, 20, 4);
                    moveSnake();
                }
            }
        }
    }
}

void Snake::spawnFood(int x, int y) {
    int rX = x * 20;
    int rY = y * 20;
    food.x = rX;
    food.y = rY;
    food.visible = true;
}

void Snake::checkFood() {

    auto &first = snk.parts.front();
    if (first.x == food.x && first.y == food.y) {
        snk.parts.emplace_back(20, 20, snk.parts.size() + 1);
        food.visible = false;
        count++;
    }
}

void Snake::setString() {

    player_text.setString("Points: " + std::to_string(count));
}
