#!/bin/sh
mkdir -p release/linux_arm64
g++ src/main.cpp src/game.cpp src/entities.cpp -lm -lsfml-graphics -lsfml-window -lsfml-system -o release/linux_arm64/maxsteroid
