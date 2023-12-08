#!/bin/sh
mkdir -p bin
g++ src/main.cpp src/game.cpp src/entities.cpp -lm -lsfml-graphics -lsfml-window -lsfml-system -o bin/maxsteroid
