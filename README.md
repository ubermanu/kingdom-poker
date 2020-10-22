# Kingdom Poker

2010 school project, written in C

## Install

    sudo apt install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev libsdl-mixer1.2-dev

## Build

    gcc SDL_rotozoom.c videopoker.c main.c `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lSDL_mixer -lm -o kingdompoker

## Run

    ./kingdompoker

## Links

* SDL - [https://www.libsdl.org/]()
* Rotozoom - [http://www.ferzkopp.net/Software/SDL_rotozoom/]()
