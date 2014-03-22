Tetris Unleashed!
================

Tetris clone based on [QuantumKing's](https://github.com/QuantumKing) deleted project.

![alt text](./screenshots/tetris-unleashed-screenshot.png "Tetris Unleashed!")

## Installation

You will need
+ [SDL 2.0.1](http://www.libsdl.org/)
+ [SDL_ttf 2.0](http://www.libsdl.org/projects/SDL_ttf/)

## How to play

Up Arrow/W      -> rotates the current tetromino

Left Arrow/A    -> moves the current tetromino to the left

Right Arrow/D   -> moves the current tetromino to the right

Down Arrow/S    -> speeds up the current tetromino

Spacebar        -> zooms the current tetromino to the bottom


New Game        -> starts new game

Quit            -> quits

## To-do

+ ~~Update code to SDL2~~
+ ~~Add fonts with SDL_ttf~~
+ Convert to OOP
+ Add music
+ Add pause option
+ Add fullscreen option 
+ Add tetrimino shadow
+ Add mouse control
+ Add menu screen

# Credits

This project is based on [QuantumKing's](https://github.com/QuantumKing) now deleted project. 

Restructuring and modifications by Chafic Najjar - chafic.najjar@gmail.com
+ Added all comments
+ Removed Tetris class, re-organized all variables and shortened code 
+ Added support for W/A/S/D keys
+ Fixed bug that didn't allow continuously shifting the tetromino left or right
+ Increased game speed
+ Replaced legacy OpenGL and SDL1.2 with pure SDL 2.0 implementation
+ Replaced FreeType with SDL2_tff
