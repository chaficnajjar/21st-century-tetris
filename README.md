Tetris Unleashed!
=================

Tetris clone for Linux written in C++ with SDL 2.0.

![Preview](https://cloud.githubusercontent.com/assets/1498164/5609348/184229dc-94a7-11e4-8979-4bac0b527aa2.gif)
![Screenshot](https://cloud.githubusercontent.com/assets/1498164/5609349/1850910c-94a7-11e4-9af4-85be530b130e.png)

## Dependencies

You will need:

+ [SDL 2.0](https://www.libsdl.org/hg.php)
+ [SDL TTF 2.0](https://www.libsdl.org/projects/SDL_ttf/)
+ [SDL image 2.0](https://www.libsdl.org/projects/SDL_image/)

### Ubuntu

Install all dependencies on Ubuntu 14.04 and above:

`sudo apt-get install libsdl2-dev libsdl2-ttf-dev libsdl2-image-dev`

## Get Source and Build

```
git clone https://github.com/chaficnajjar/tetris-unleashed.git
cd tetris-unleashed
make
```

## Run the game

To run the game:

`./tetris`.

## How to play

Up arrow/w      -> rotates the current tetromino

Left arrow/a    -> moves the current tetromino to the left

Right arrow/d   -> moves the current tetromino to the right

Down arrow/s    -> speeds up the current tetromino

Spacebar        -> zooms the current tetromino to the bottom

p               -> pauses/resumes game

New Game        -> starts new game

Quit            -> quits

## Contribute

If you would like to contribute, please make sure you follow the [Google C++ Style Guide](http://google-styleguide.googlecode.com/svn/trunk/cppguide.html).

## License and Credits

```
The MIT License (MIT)

Copyright (c) 2014 Chafic Najjar

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
```

Chafic Najjar - chafic.najjar@gmail.com.

[irrKlang](http://www.ambiera.com/irrklang/index.html) for sound.

