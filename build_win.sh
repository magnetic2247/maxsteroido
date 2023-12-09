#!/bin/sh
mkdir -p release/windows/
x86_64-w64-mingw32-g++ src/main.cpp src/game.cpp src/entities.cpp -D SFML_STATIC -D M_PIl=M_PI -Ilibraries/include/ -Ilibraries/SFML/include/ -Llibraries/SFML/lib/ -static -lm -lsfml-graphics-s -lfreetype -lsfml-window-s -lopengl32 -lgdi32 -lsfml-system-s -lwinmm -o release/windows/maxsteroid.exe
