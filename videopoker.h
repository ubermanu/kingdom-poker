#ifndef VIDEOPOKER_H
#define VIDEOPOKER_H
    typedef struct Card
    {
        int x;
        int y;
        int width;
        int value;
        int select;
    }Card;
    typedef struct Token
    {
        int x;
        int y;
        int width;
        int offx;
        int offy;
        int index;
        int value;
    }Token;
    typedef struct Cursor
    {
        int x;
        int y;
        int state;
        int drag;
        SDL_Surface *sfc;
        SDL_Rect spr[2];
    }Cursor;
    void BlendDeck(int *array);
	void ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect* sprite);
	int CheckCardCombo(Card *a_tmp);
#endif
