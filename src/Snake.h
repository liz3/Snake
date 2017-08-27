//
// Created by Yann Holme Nielsen on 27.08.17.
//

#ifndef SNAKE_SNAKE_H
#define SNAKE_SNAKE_H

#include <SFML/Graphics.hpp>


struct Gitter {
    unsigned columns, rows;
};
struct Food {
    int y{};
    int x{};
    bool visible = false;
};
struct BodyPart {
    BodyPart(unsigned x, unsigned y, int index):
            x (x), y (y), index (index){

    }
    int64_t x, y, oldX{}, oldY{};
    int index;

};
struct Snk {
    std::vector<BodyPart> parts = std::vector<BodyPart>();
};
class Snake {
public:
    Snake(unsigned width, unsigned height);

private:
    int count = 0;
    sf::Font m_font;
    sf::Text player_text;
    bool changed = false;
    Gitter gitter;
    int xOffset = 0;
    int yOffset = 0;
    void run();
    void checkInput();
    void moveSnake();
    void checkCollision();
    void spawnFood(int x, int y);
    void setString();
    void drawRectangle(float x, float y, float width, float height);
    void checkFood();
    sf::RenderWindow window;
    std::vector<sf::Vertex> m_objects;
    Snk snk;
    Food food;

};


#endif //SNAKE_SNAKE_H
