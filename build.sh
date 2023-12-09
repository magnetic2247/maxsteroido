#!/bin/sh
mkdir -p release/linux_amd64
g++ src/main.cpp src/game.cpp src/entities.cpp -lm -lsfml-graphics -lsfml-window -lsfml-system -Ofast -o release/linux_amd64/maxsteroid
