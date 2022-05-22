#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cstdlib>
#include <thread>
#include <istream>
#include <iostream>
#include <functional>
#include "Cell.h"
#include "main.h"

void initializeCellMap(std::vector<std::vector<myObjs::Cell>> *cellMap) {
    for(int i = 0; i < COLUMS_NUMBER; i++) {
        for(int j = 0; j < ROWS_NUMBER; j++) {
            (*cellMap)[i][j].setPos(i * CELL_SIZE, j * CELL_SIZE);
            if((i % 2 == 0 || j % 2 == 0) && rand() % 2 == 0)
                (*cellMap)[i][j].revive();
        }
    }
}

void cleanCellMap(std::vector<std::vector<myObjs::Cell>> *cellMap) {
    for(int i = 0; i < COLUMS_NUMBER; i++) {
        for(int j = 0; j < ROWS_NUMBER; j++) {
            (*cellMap)[i][j].kill();
        }
    }
}

void drawCellMap(sf::RenderWindow *window, std::vector<std::vector<myObjs::Cell>> *cellMap) {
    for(int i = 0; i < COLUMS_NUMBER; i++) {
        for(int j = 0; j < ROWS_NUMBER; j++) {
            (*window).draw((*cellMap)[i][j].getShape());
        }
    }
}

void placeCellInMap(sf::Vector2i mousePos, std::vector<std::vector<myObjs::Cell>> *cellMap) {
    if(mousePos.x >= 0 && mousePos.x <= WINDOW_WIDTH  && mousePos.y >= 0 && mousePos.y <= WINDOW_HEIGHT) {
        int cellXPos = (int) mousePos.x/CELL_SIZE; 
        int cellYPos = (int) mousePos.y/CELL_SIZE; 
        (*cellMap)[cellXPos][cellYPos].revive();
    }
}

std::vector<std::vector<myObjs::Cell>> setNextGenMap(std::vector<std::vector<myObjs::Cell>> *currCellMap, 
        std::vector<std::vector<myObjs::Cell>> *nextCellMap) {

    std::vector<std::thread> vecOfThreads;

    std::function<void(std::vector<std::vector<myObjs::Cell>>*,
                       std::vector<std::vector<myObjs::Cell>>*,
                       int)> func = 
        [](std::vector<std::vector<myObjs::Cell>> *currCellMap,
           std::vector<std::vector<myObjs::Cell>> *nextCellMap,
           int line) {
                for(int i = 0; i < ROWS_NUMBER; i++) {
                    int neighbors = 0;
                    for(int j = -1; j <= 1; j++) {
                        for(int k = -1; k <= 1; k++) {
                            if(!(j == 0 && k == 0) && 
                                line + j >= 0 && line + j < COLUMS_NUMBER && i + k >= 0 && i + k < ROWS_NUMBER && 
                                (*currCellMap)[line + j][i + k].alive())
                                neighbors++;
                        }
                    }
                    
                    if((*currCellMap)[line][i].alive()) {
                        if(neighbors == 2 || neighbors == 3)
                            (*nextCellMap)[line][i].revive();
                        else
                            (*nextCellMap)[line][i].kill();
                    } else {
                        if(neighbors == 3)
                            (*nextCellMap)[line][i].revive();
                        else
                            (*nextCellMap)[line][i].kill();
                    }
                }
        };

    for(int i = 0; i < COLUMS_NUMBER; i++) {
        vecOfThreads.push_back(std::thread(func, currCellMap, nextCellMap, i)); 
    }

    std::vector<std::thread>::iterator it;
    for(auto it = begin (vecOfThreads); it != end (vecOfThreads); ++it) {
        it->join();
    }

    return *nextCellMap;
}

int main() {
    bool paused = false;

    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "My window");
    window.setFramerateLimit(FPS);
    window.setVerticalSyncEnabled(true);

    std::vector<std::vector<myObjs::Cell>> currentGen(COLUMS_NUMBER, std::vector<myObjs::Cell>(ROWS_NUMBER));
    initializeCellMap(&currentGen);

    std::vector<std::vector<myObjs::Cell>> nextGen(COLUMS_NUMBER, std::vector<myObjs::Cell>(ROWS_NUMBER));
    initializeCellMap(&nextGen);
    
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
                initializeCellMap(&currentGen);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
                paused = true;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
                paused = false;
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
                cleanCellMap(&currentGen);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                placeCellInMap(sf::Mouse::getPosition(window), &currentGen);
        }

        // render
        window.clear(sf::Color::Black);
        drawCellMap(&window, &currentGen);

        if(!paused)
            currentGen = setNextGenMap(&currentGen, &nextGen); 

        window.display();
    }

    return 0;
}
