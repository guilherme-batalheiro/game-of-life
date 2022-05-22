sfml-app: main.o myObjs.o
	g++ main.o Cell.o -o game-of-life -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	g++ -c main.cpp

myObjs.o: Cell.cpp Cell.h
	g++ -c Cell.cpp

clean:
	rm *.o
