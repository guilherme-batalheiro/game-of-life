#include "Cell.h"
#include <iostream>

// tile

myObjs::Cell::Cell() {
    _shape = sf::RectangleShape(sf::Vector2f(_size, _size));
    _shape.setFillColor(sf::Color::Black);
}

myObjs::Cell::Cell(float xPos, float yPos) {
    _shape = sf::RectangleShape(sf::Vector2f(_size, _size));

    _shape.setPosition(sf::Vector2f(xPos, yPos));
    _shape.setFillColor(sf::Color::Black);
}

void myObjs::Cell::setPos(float xPos, float yPos) {
    _shape.setPosition(sf::Vector2f(xPos, yPos));
}

sf::Vector2f myObjs::Cell::getPos() {
    return _shape.getPosition();
}

float myObjs::Cell::getSize() {
    return _size;
}

sf::RectangleShape myObjs::Cell::getShape() {
    return _shape;
}

bool myObjs::Cell::alive() {
    return _alive;
}

void myObjs::Cell::kill() {
    if(_alive) {
        _alive = false;
        _shape.setFillColor(sf::Color::Black);
    }
}

void myObjs::Cell::revive() {
    if(!_alive) {
        _alive = true;
        _shape.setFillColor(sf::Color::Green);
        _shape.setOutlineThickness(0);
    }
}

