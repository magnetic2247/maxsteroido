#!/bin/sh
mkdir -p bin
cd src/
g++ -lm -lsfml-graphics -lsfml-window -lsfml-system main.cpp entities.cpp -o ../bin/maxsteroid
