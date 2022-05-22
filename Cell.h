#ifndef _CELL_
#define _CELL_

#include "main.h"
#include <SFML/Graphics.hpp>

namespace myObjs {
    class Cell {
        private:
            float _size = CELL_SIZE;
            bool _alive = false;
            sf::RectangleShape _shape;

        public:
            Cell();
            Cell(float xPos, float yPos);

            float getSize();

            void setPos(float xPos, float yPos);
            sf::Vector2f getPos();

            void setShape(sf::RectangleShape shape);
            sf::RectangleShape getShape();

            bool alive();
            void kill();
            void revive();
    };
}
#endif
