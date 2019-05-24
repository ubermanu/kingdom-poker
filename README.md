# Kingdom Poker

## Install

    sudo apt install libsdl1.2-dev libsdl-ttf2.0-dev libsdl-image1.2-dev

## Build

    gcc videopoker.c main.c `sdl-config --cflags --libs` -lSDL_ttf -lSDL_image -lm -o kingdompoker

## Run

    ./kingdompoker
