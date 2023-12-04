#!/bin/sh
mkdir -p bin
g++ -lm -lsfml-graphics -lsfml-window -lsfml-system src/main.cpp src/game.cpp src/entities.cpp -o bin/maxsteroid
