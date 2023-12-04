#!/bin/sh
cd src/
g++ -g -lm -lsfml-graphics -lsfml-window -lsfml-system main.cpp entity.cpp asteroid.cpp spaceship.cpp laser.cpp -o ../bin/maxsteroid
