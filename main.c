#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_mixer.h>
#include "SDL_rotozoom.h"

#include "main.h"
#include "videopoker.h"

#define true 1
#define false 0

int i = 0, j = 0, k = 0;

SDL_Event event;

Card objCard[5];
Token objToken[50];
Cursor objCursor;

SDL_Color white = {255, 255, 255}, darkgrey = {32, 32, 32};

SDL_Surface *screen = NULL, *background = NULL, *sfcText = NULL, *sfcCard = NULL, *frontCard = NULL, *backCard = NULL, *sfcDeck = NULL, *backDeck = NULL, *sfcToken = NULL, *numbers = NULL, *score = NULL, *mise = NULL, *heads = NULL, *color = NULL, *shadow = NULL, *shadowPart = NULL, *title = NULL, *sfcMenu = NULL, *sfcMisc = NULL, *sfcHelp = NULL;
int cntToken = 5, sprEmoIndex = 0, imgCardIndex = 0;
int done = 0, state = 0, game_stop = 0, pot = 0, bank = 0, shwSelected = 0, title_alpha = 255, gameResult = 0, helped = 0;
int Deck[52] = {0};

SDL_Rect sprToken[5] = {0,0}, sprHeads[24] = {0,0}, sprNumbers[18] = {0,0}, sprColor[4] = {0,0}, sprDeck[2] = {0,0}, sprMenu[4] = {0,0}, sprMisc[6] = {0,0};
TTF_Font *fntList[3] = {NULL};

Mix_Chunk *sndCard = NULL, *sndToken = NULL, *sndCoin = NULL;
Mix_Music *music = NULL, *noise = NULL;

FILE * savFile;

int main(int argc, char *argv[])
{
	SDL_Init(SDL_INIT_VIDEO);
	atexit(SDL_Quit);
    TTF_Init();

	//Cr�ation de la fen�tre principale
	screen = SDL_SetVideoMode(880, 440, 32, SDL_HWSURFACE|SDL_DOUBLEBUF);
	SDL_WM_SetCaption("Kingdom Poker", NULL);
	SDL_WM_SetIcon(IMG_Load("data/img/icon.png"), NULL); //32*32

    //Initialisation g�n�rale du jeu
	VIDEOPOKER_Init();

	while (!done)
	{
		//cadencer le jeu
		SDL_Delay(10);
		//Dessiner l'arri�re plan graphique
		ApplySurface(0, 0, background, screen, NULL);
		//V�rifier l'�tat et l'instant du jeu
		VIDEOPOKER_Game();
		//D�placer et animer le curseur
		ApplySurface(objCursor.x - objCursor.state * 10, objCursor.y - objCursor.state * 15, objCursor.sfc, screen, &objCursor.spr[objCursor.state]);
		//Actualiser l'�cran
		SDL_Flip(screen);
		//V�rifier les int�ractions avec le joueur
		CheckEvent();
	}

    //lib�ration de la m�moire et fermeture des librairies utilis�es
	VIDEOPOKER_Quit();
	return 0;
}

void DrawCard()
{
    for (i = 0; i < 5; i++)
	{
		switch (objCard[i].select)
		{
			case 1:
			{
                ApplySurface(objCard[i].x + 5, objCard[i].y + 2, sfcCard, screen, NULL);
                ApplySurface(objCard[i].x + 10, objCard[i].y + 14, color, screen, &sprColor[objCard[i].value/13]);
				switch (objCard[i].value % 13)
				{
					case 0: ApplySurface(objCard[i].x + 20, objCard[i].y + 15, heads, screen, &sprHeads[objCard[i].value/13]); break;
					case 10: ApplySurface(objCard[i].x + 20, objCard[i].y + 15, heads, screen, &sprHeads[6 + sprEmoIndex + 3 * ((objCard[i].value/13+1)%2)]); break;
					case 11: ApplySurface(objCard[i].x + 20, objCard[i].y + 15, heads, screen, &sprHeads[12 + sprEmoIndex + 3 * ((objCard[i].value/13+1)%2)]); break;
					case 12: ApplySurface(objCard[i].x + 20, objCard[i].y + 15, heads, screen, &sprHeads[18 + sprEmoIndex + 3 * ((objCard[i].value/13+1)%2)]); break;
					default: ApplySurface(objCard[i].x + 38, objCard[i].y + 50, numbers, screen, &sprNumbers[((objCard[i].value % 13) - 1) + 9 * ((objCard[i].value/13+1)%2)]);
				}
			}
			break;
			case -1:
			{
				ApplySurface(objCard[i].x + 5, objCard[i].y + 2, backCard, screen, NULL);
			}
			break;
		}
        ApplySurface(objCard[i].x, objCard[i].y, frontCard, screen, NULL);
	}
}

void DrawToken()
{
    for (i = 0; i < cntToken; i++)
    {
        if ((i < 5 && bank >= objToken[i].value) || i >= 5)
        {
            ApplySurface(objToken[i].x, objToken[i].y, sfcToken, screen, &sprToken[objToken[i].index]);
        }
    }
}

void DrawScore()
{
    char *score_buffer = malloc(10*sizeof(char));
	sprintf(score_buffer, "%d", bank);
	SDL_FreeSurface(score);
	score = TTF_RenderText_Blended(fntList[0], score_buffer, white);
	sprintf(score_buffer, "%d", pot);
	SDL_FreeSurface(mise);
	mise = TTF_RenderText_Blended(fntList[1], score_buffer, white);
	ApplySurface(720, 10, sfcMisc, screen, &sprMisc[0]);
	ApplySurface(720, 55, sfcMisc, screen, &sprMisc[1]);
	ApplySurface(780, 10, score, screen, NULL);
	ApplySurface(780, 50, mise, screen, NULL);
	free(score_buffer);
}

void DrawText(char* text)
{
	int x = 215;
	int y = 50;
	SDL_FreeSurface(sfcText);
	sfcText = TTF_RenderText_Blended(fntList[2], text, darkgrey);
    SDL_SetAlpha(sfcText, SDL_SRCALPHA, 10);
	ApplySurface(x+2, y+3, sfcText, screen, NULL);
	SDL_FreeSurface(sfcText);
	sfcText = TTF_RenderText_Blended(fntList[2], text, white);
	ApplySurface(x, y, sfcText, screen, NULL);
}

void CheckEvent()
{
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT: done++; break;
			case SDL_KEYDOWN:
			{
				switch (event.key.keysym.sym)
				{
					case SDLK_ESCAPE:
					if (state != 0)
					{
						if (game_stop == 1)
						{
							game_stop = 0;
							Mix_PauseMusic();
						}
						else
						{
							objCursor.drag = -1;
							game_stop = 1;
							Mix_ResumeMusic();
						}
					}
					break;
				}
			}
			break;
			//BOUTON ENFONCE DE LA SOURIS
			case SDL_MOUSEBUTTONDOWN:
            {
                if (game_stop == false && helped == false)
                {
                    objCursor.drag = -1;
                    for (i = cntToken - 1; i >= 0; i--)
                    {
                        if (MouseOnToken(objToken[i]) == true && event.button.button == SDL_BUTTON_LEFT && state == 1 && game_stop == 0)
                        {
                            objToken[i].offx = event.button.x - objToken[i].x;
                            objToken[i].offy = event.button.y - objToken[i].y;
                            if (i < 5 && bank >= objToken[i].value)
                            {
                                objToken[cntToken] = objToken[i];
                                ApplySurface(objToken[cntToken].x, objToken[cntToken].y, sfcToken, screen, NULL);
                                objCursor.drag = cntToken;
                                cntToken++;
                                break;
                            }
                            if (i >= 5)
                            {
                                for (j = i; j < cntToken - 1; j++)
                                {
                                    Token tmp = objToken[j];
                                    objToken[j] = objToken[j+1];
                                    objToken[j+1] = tmp;
                                }
                                objCursor.drag = cntToken - 1;
                                break;
                            }
                            break;
                        }
                    }
                }
            }
            break;
			//CLIC DE LA SOURIS
            case SDL_MOUSEBUTTONUP:
            {
				if (game_stop == false)
				{
				    if (helped == false)
				    {
                        switch (state)
                        {
                            case 0:
                            {
                                if (event.button.button == SDL_BUTTON_LEFT)
                                {
                                    if (shwSelected != 0)
                                    {
                                        //charger partie
                                        savFile = fopen("data/sav.ini","r");
                                        if (savFile != NULL)
                                        {
                                            char *sav_buffer = malloc(32*sizeof(char));
                                            char* data_buffer;
                                            data_buffer = fgets(sav_buffer, 32, savFile);
                                            bank = atoi(data_buffer);
                                            free(sav_buffer);
                                            free(data_buffer);
                                            if (bank == 0) bank = 50;
                                            if (bank > 99999) bank = 99999;
                                        }
                                    }
                                    state = 10;
                                }
                            }
                            break;
                            case 1:
                            {
                                for (i = cntToken - 1; i >= 0; i--)
                                {
                                    if (MouseOnToken(objToken[i]) == true && event.button.button == SDL_BUTTON_RIGHT && objCursor.drag == -1)
                                    {
                                        if (i < 5 && bank >= objToken[i].value)
                                        {
                                            objToken[cntToken] = objToken[i];
                                            objToken[cntToken].x = 300 + rand() % 480;
                                            objToken[cntToken].y = 100 + rand() % 280;
                                            ApplySurface(objToken[cntToken].x, objToken[cntToken].y, sfcToken, screen, NULL);
                                            objCursor.drag = cntToken;
                                            cntToken++;
                                            break;
                                        }
                                        else if (i >= 5 && objCursor.drag == -1)
                                        {
                                            for (j = i; j < cntToken - 1; j++)
                                            {
                                                Token tmp = objToken[j];
                                                objToken[j] = objToken[j+1];
                                                objToken[j+1] = tmp;
                                            }
                                            pot -= objToken[cntToken - 1].value;
                                            bank += objToken[cntToken - 1].value;
                                            objCursor.drag = -1;
                                            cntToken--;
                                            break;
                                        }
                                    }
                                }
                                if (objCursor.drag > -1)
                                {
                                    bank += pot;
                                    pot = 0;
                                    for (i = 5; i < cntToken; i++) pot += objToken[i].value;
                                    bank -= pot;
                                    if (Mix_PausedMusic() != 1) Mix_PlayChannel(-1, sndToken, 0);
                                }
                                if (MouseOnDeck() == true && objCursor.drag == -1 && pot > 0)
                                {
                                    state = 2;
                                }
                                objCursor.drag = -1;
                            }
                            break;
                            case 3: //Int�ractions � la distribution des cartes
                            {
                                for (i = 0; i < 5; i++)
                                {
                                    switch (event.button.button)
                                    {
                                        case SDL_BUTTON_LEFT:
                                        {
                                            if (MouseOnCard(objCard[i]) == true)
                                            {
                                                objCard[i].select *= -1;
                                                objCard[i].y += objCard[i].select * 20;
                                                if (Mix_PausedMusic() != 1) Mix_PlayChannel(-1, sndCard, 0);
                                            }
                                        }
                                        break;
                                        case SDL_BUTTON_WHEELUP:
                                        {
                                            if (objCard[i].select == 1)
                                            {
                                                objCard[i].select = -1;
                                                objCard[i].y += objCard[i].select * 20;
                                                if (Mix_PausedMusic() != 1) Mix_PlayChannel(-1, sndCard, 0);
                                            }
                                            objCard[i].select = -1;
                                        }
                                        break;
                                        case SDL_BUTTON_WHEELDOWN:
                                        {
                                            if (objCard[i].select == -1)
                                            {
                                                objCard[i].select = 1;
                                                objCard[i].y += objCard[i].select * 20;
                                                if (Mix_PausedMusic() != 1) Mix_PlayChannel(-1, sndCard, 0);
                                            }
                                            objCard[i].select = 1;
                                        }
                                        break;
                                        case SDL_BUTTON_MIDDLE:
                                        {
                                            objCard[i].select *= -1;
                                            objCard[i].y += objCard[i].select * 20;
                                           if (Mix_PausedMusic() != 1) Mix_PlayChannel(-1, sndCard, 0);
                                        }
                                        break;
                                    }
                                }
                                if (MouseOnDeck() == true && event.button.button == SDL_BUTTON_LEFT)
                                {
                                    state = 4;
                                }
                            }
                            break;
                            case 4:
                            {
                                if (bank > 0)
                                {
                                    if (pot == 0) state = 1;
                                }
                                else
                                {
                                    state = 0;
                                    pot = 0;
                                    bank = 50;
                                    BlendDeck(&Deck);
                                    SDL_FreeSurface(title);
                                    title = SDL_LoadBMP("data/img/title.bmp");
                                    title_alpha = 255;
                                }
                            }
                            break;
                        }
                        if (MouseOnMute() == true && objCursor.drag == -1 && state != 0)
                        {
                            if (Mix_PausedMusic()) Mix_ResumeMusic();
                            else Mix_PauseMusic();
                        }
                        if (MouseOnHelp() == true && objCursor.drag == -1 && state != 0)
                        {
                            helped = true;
                        }
				    }
				    else
				    {
                        helped = 0;
				    }
				}
				else
				{
				    if (event.button.x < 440)
				    {
				        //sauvegarder le score courant
                        savFile = fopen("data/sav.ini","w");
                        if (savFile != NULL)
                        {
                            char *sav_buffer=malloc(32*sizeof(char));
                            switch (state)
                            {
                                case 4: sprintf(sav_buffer, "%d", bank + pot); break;
                                default: sprintf(sav_buffer, "%d", bank + pot * gameResult);
                            }
                            fputs(sav_buffer, savFile);
                            fclose(savFile);
                            free(sav_buffer);
                        }
                        //reprendre le jeu
				        game_stop = 0;
                        // FSOUND_SetPaused(FSOUND_ALL, 0);
				    }
				    else done++;
				}
			}
            break;
			//MOUVEMENT DE LA SOURIS
            case SDL_MOUSEMOTION:
            {
				objCursor.x = event.button.x;
				objCursor.y = event.button.y;
				if (objCursor.drag == -1)
				{
                    objCursor.state = 0;
                    if (state == 0 || game_stop == 1)
                    {
                        if (event.button.x < 440) shwSelected = 0;
                        else shwSelected = 1;
                    }
				}
				else
				{
				    if (state == 1 && event.button.button == SDL_BUTTON_LEFT)
                    {
                        objToken[objCursor.drag].x = event.button.x - objToken[objCursor.drag].offx;
                        objToken[objCursor.drag].y = event.button.y - objToken[objCursor.drag].offy;
                    }
				    objCursor.state = 1;
				}
            }
            break;
		}
	}
}

void VIDEOPOKER_Init()
{
    //initialisations des biblioth�ques annexes
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);

	srand(time(NULL));

    //initialisation de la partie
	for (i = 0; i < 52; i++) Deck[i] = i;
    pot = 0;
    bank = 50;

    //initialisation du fond du jeu et de l'�can d'accueil
	background = SDL_LoadBMP("data/img/background.bmp");
	title = SDL_LoadBMP("data/img/title.bmp");
	sfcHelp = SDL_LoadBMP("data/img/help.bmp");

    //ombre port�e
	shadow = SDL_CreateRGBSurface(SDL_HWSURFACE, 880, 480, 32, 0, 0, 0, 0);
	SDL_FillRect(shadow, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_SetAlpha(shadow, SDL_SRCALPHA, 128);

    //ombre d�signant la partie du menu s�l�ctionn�e
	shadowPart = SDL_CreateRGBSurface(SDL_HWSURFACE, 440, 480, 32, 0, 0, 0, 0);
	SDL_FillRect(shadowPart, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
	SDL_SetAlpha(shadowPart, SDL_SRCALPHA, 64);

    //init de l'objet Curseur
	objCursor.sfc = IMG_Load("data/img/cursor.png");
	SDL_ShowCursor(SDL_DISABLE);
	for (i = 0; i < 2; i++)
	{
		objCursor.spr[i].x = i * 33;
		objCursor.spr[i].y = 0;
		objCursor.spr[i].w = 33;
		objCursor.spr[i].h = 38;
	}
	objCursor.x = 0;
	objCursor.y = 0;
	objCursor.state = 0;
	objCursor.drag = -1;

    //init de l'objet Cartes
    sfcCard = SDL_CreateRGBSurface(SDL_HWSURFACE, 140, 140, 32, 0, 0, 0, 0);
	SDL_FillRect(sfcCard, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
    for (i = 0; i < 5; i++)
    {
        objCard[i].x = 20 + i * (150 + 20);
        objCard[i].y = 120;
        objCard[i].width = 150;
        objCard[i].value = 0;
        objCard[i].select = 1;
    }

	//Bordure avant des cartes
	frontCard = IMG_Load("data/img/frontcard.png");

	//Dos des cartes et arri�re plan pour le dos du deck selon le fond des cartes
	backCard= IMG_Load("data/img/backcard.bmp");
	backDeck = rotozoomSurface(backCard, 15, 1.0, 1);

	//Paquet de carte (bouton haut gauche)
	sfcDeck = IMG_Load("data/img/deck.png");
	for (i = 0; i < 2; i++)
	{
		sprDeck[i].x = i * 179;
		sprDeck[i].y = 0;
		sprDeck[i].w = 179;
		sprDeck[i].h = 180;
	}

	//Menu
	sfcMenu = IMG_Load("data/img/menu.png");
	for (i = 0; i < 4; i++)
	{
		sprMenu[i].x = 0;
		sprMenu[i].y = i * 100;
		sprMenu[i].w = 200;
		sprMenu[i].h = 100;
	}

	//Activer, d�sactiver le son
	sfcMisc = IMG_Load("data/img/misc.png");
	for (i = 0; i < 6; i++)
	{
		sprMisc[i].x = (i % 2) * 50;
		sprMisc[i].y = (i / 2) * 50;
		sprMisc[i].w = 50;
		sprMisc[i].h = 50;
	}

	//Jetons et valeurs
	sfcToken = IMG_Load("data/img/token.png");
	for (i = 0; i < cntToken; i++)
	{
		objToken[i].x = 150 + i * (115 + 20);
		objToken[i].y = 310;
		objToken[i].width = 115;
		objToken[i].offx = 0;
		objToken[i].offy = 0;
		objToken[i].index = i;

		sprToken[i].x = i * 115;
		sprToken[i].y = 0;
		sprToken[i].w = 115;
		sprToken[i].h = 115;
	}

	objToken[0].value = 1;
	objToken[1].value = 5;
	objToken[2].value = 10;
	objToken[3].value = 20;
	objToken[4].value = 100;

	//Couleurs (Pique, Coeur, Trefle, Carreau)
	color = IMG_Load("data/img/color.png");
	for (i = 0; i < 4; i++)
	{
		sprColor[i].x = i * 39;
		sprColor[i].y = 0;
		sprColor[i].w = 39;
		sprColor[i].h = 40;
	}

	//T�tes (As, Valet, Dame, Roi)
	heads = IMG_Load("data/img/heads.png");
	for (i = 0; i < 24; i++)
	{
		sprHeads[i].x = (i % 6) * 120;
		sprHeads[i].y = (i / 6) * 125;
		sprHeads[i].w = 120;
		sprHeads[i].h = 125;
	}

	//Nombres (images centrales des cartes)
	numbers = IMG_Load("data/img/numbers.png");
	for (i = 0; i < 18; i++)
	{
		sprNumbers[i].x = (i % 9) * 100;
		sprNumbers[i].y = (i / 9) * 80;
		sprNumbers[i].w = 100;
		sprNumbers[i].h = 80;
	}

    //Polices
    fntList[0] = TTF_OpenFont("data/fnt/cloisterblack.ttf", 36);
    fntList[1] = TTF_OpenFont("data/fnt/cloisterblack.ttf", 48);
    fntList[2] = TTF_OpenFont("data/fnt/cloisterblack.ttf", 42);

    //Musique et sons
	sndCard = Mix_LoadWAV("data/snd/card.ogg");
	sndToken = Mix_LoadWAV("data/snd/token.ogg");
	sndCoin = Mix_LoadWAV("data/snd/coin.ogg");

    music = Mix_LoadMUS("data/snd/music.ogg");
	Mix_PlayMusic( music, -1 );

    // noise = Mix_LoadMUS("data/snd/noise.ogg");
	// Mix_PlayMusic( noise, -1 );
}

void VIDEOPOKER_Quit()
{
	//Lib�rer la m�moire
	SDL_FreeSurface(background);
	SDL_FreeSurface(sfcText);
	SDL_FreeSurface(sfcCard);
	SDL_FreeSurface(frontCard);
	SDL_FreeSurface(backCard);
	SDL_FreeSurface(sfcToken);
	SDL_FreeSurface(sfcDeck);
	SDL_FreeSurface(backDeck);
	SDL_FreeSurface(score);
	SDL_FreeSurface(mise);
	SDL_FreeSurface(numbers);
	SDL_FreeSurface(heads);
	SDL_FreeSurface(objCursor.sfc);
	SDL_FreeSurface(color);
	SDL_FreeSurface(shadow);
	SDL_FreeSurface(shadowPart);
	SDL_FreeSurface(sfcMenu);
	SDL_FreeSurface(sfcMisc);
	SDL_FreeSurface(sfcHelp);

	TTF_CloseFont(fntList[0]);
	TTF_CloseFont(fntList[1]);
	TTF_CloseFont(fntList[2]);

	Mix_FreeChunk(sndCard);
	Mix_FreeChunk(sndToken);
	Mix_FreeChunk(sndCoin);
	Mix_FreeMusic(music);
	Mix_FreeMusic(noise);

	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}

void VIDEOPOKER_Game()
{
	if (game_stop == false)
	{
	    //dessiner le packet de carte
		ApplySurface(-20, - 45, backDeck, screen, NULL);
		if (Mix_PausedMusic() == 1) ApplySurface(-20, -45, sfcDeck, screen, &sprDeck[1]);
		else ApplySurface(-20, -45, sfcDeck, screen, &sprDeck[0]);

        //afficher le score
		DrawScore();
		//afficher le bouton de mute
		if (Mix_PausedMusic() == 1) ApplySurface(830, 390, sfcMisc, screen, &sprMisc[5]);
		else  ApplySurface(830, 390, sfcMisc, screen, &sprMisc[4]);
		//afficher le bouton d'aide
		if (MouseOnHelp() == true) ApplySurface(0, 390, sfcMisc, screen, &sprMisc[3]);
		else ApplySurface(0, 390, sfcMisc, screen, &sprMisc[2]);
		//afficher l'etape du jeu
		switch (state)
		{
			case 0:
            {
                //Etape de menu
                ApplySurface(0, 0, title, screen, NULL);
                ApplySurface(0, 0, shadow, screen, NULL);
                ApplySurface(shwSelected * 440, 0, shadowPart, screen, NULL);
                ApplySurface(110, 350, sfcMenu, screen, &sprMenu[0]);
                ApplySurface(550, 350, sfcMenu, screen, &sprMenu[3]);
            }
            break;
            case 10:
            {
                DrawToken();
                if (title_alpha > 0)
                {
                    SDL_SetAlpha(title, SDL_SRCALPHA, title_alpha);
                    ApplySurface(0, 0, title, screen, NULL);
                    title_alpha -= 15;
                }
                else state = 1;
            }
            break;
			case 1:
			{
				//Etape de mise
				if (bank > 0) DrawText("Veuillez miser...");
				else
				{
					if (pot > 0) DrawText("Vous n'avez plus de jetons!");
					else DrawText("La partie est termin�e!");
				}
				sprEmoIndex = 0;
				DrawToken();
			}
			break;
			case 2:
			{
				DrawToken();
			    if (cntToken > 5)
			    {
			        //anmations d'empochage des jetons par la banque
                    for (i = 5; i < cntToken; i++)
                    {
                        if (objToken[i].y > -120)
                        {
                            objToken[i].y -= 30;
                            if (objToken[i].y <= -120)
                            {
                                objToken[i] = objToken[cntToken - 1];
                                cntToken--;
                            }
                        }
                    }
			    }
			    else
			    {
                    //Distribuer les cartes
                    BlendDeck(&Deck);
                    for (i = 0; i < 5; i++)
                    {
                        objCard[i].y = -150;
                        objCard[i].value = Deck[i];
                        objCard[i].select = 1;
                    }
                    state = 3;
			    }
			}
			break;
			case 3:
			{
				DrawText("Changer certaines cartes?");
				//Afficher les cartes
				for (i = 0; i < 5; i++)
				{
				    if (objCard[i].y < 150 + objCard[i].select * 20)
				    {
				        objCard[i].y += 30;
				    }
				}
				DrawCard();
			}
			break;
			case 4:
			{
				//Animation du changement des cartes souhait�
				for (i = 0; i < 5; i++)
				{
					if (objCard[i].select == -1)
					{
						if (objCard[i].y > -150 + objCard[i].select * 20)
						{
						    objCard[i].y -= 30;
						}
						else
						{
							objCard[i].value = Deck[i + 5];
							objCard[i].select = 1;
							if (Mix_PausedMusic() != 1) Mix_PlayChannel( -1, sndCard, 0 );
						}
					}
					else
					{
						if (objCard[i].y < 150 + objCard[i].select * 20)
						{
						    objCard[i].y += 25;
						}
					}
				}
				gameResult = CheckCardCombo(&objCard);
				//Afficher le r�sultat du tour jou�
				if (objCard[0].select > 0 && objCard[1].select > 0 && objCard[2].select > 0 && objCard[3].select > 0 && objCard[4].select > 0)
				{
					switch (gameResult)
					{
						case 1: DrawText("La paire sauve la mise!"); break;
						case 2: DrawText("Double paire!"); break;
						case 3: DrawText("C'est un Brelan!"); break;
						case 4: DrawText("Une Quinte!"); break;
						case 5: DrawText("Bien jou�, Flush!"); break;
						case 6: DrawText("Quelle chance! Full!"); break;
						case 9: DrawText("C'est bien un Carr�!"); break;
						case 11: DrawText("Une Quinte Flush!"); break;
						case 21: DrawText("Quinte Flush Royale! Woaw!"); break;
						default: DrawText("Perdu, on retente sa chance?"); sprEmoIndex = 2;
					}
					if (gameResult > 0) sprEmoIndex = 1;
					//on limite le score maximum � 99999
					if (pot != 0)
					{
						pot--;
						if (bank < 99999)
						{
						    bank += gameResult;
						}
						else
						{
						    bank = 99999;
						    pot = 0;
						}
                        //jouer un son de pi�ce
                        if (gameResult > 0 && pot % 10 == 0 && Mix_PlayingMusic()) Mix_PlayChannel( -1, sndCoin, 0 );
					}
				}
				DrawCard();
			}
			break;
		}
		//affichage de l'aide
		if (helped == true) ApplySurface(0, 0, sfcHelp, screen, NULL);
	}
	else
	{
	    //si le jeu est stopp�, on v�rifie l'etat du jeu pour n'afficher que l'element en cours de traitement
		ApplySurface(0, 0, shadow, screen, NULL);
		ApplySurface(shwSelected * 440, 0, shadowPart, screen, NULL);
		ApplySurface(110, 350, sfcMenu, screen, &sprMenu[2]);
		ApplySurface(550, 350, sfcMenu, screen, &sprMenu[1]);
	}
}

int MouseOnToken(Token object)
{
	if (sqrt(pow(object.x + object.width / 2 - event.button.x, 2) + pow(object.y + object.width / 2 - event.button.y, 2)) < object.width / 2) return true;
	else return false;
}

int MouseOnCard(Card object)
{
	if (event.button.x > object.x && event.button.x < object.x + object.width && event.button.y > object.y && event.button.y < object.y + object.width) return true;
	else return false;
}

int MouseOnDeck()
{
	if (event.button.x > 0 && event.button.x < 180 && event.button.y > 0 && event.button.y < 160) return true;
	else return false;
}

int MouseOnMute()
{
	if (event.button.x > 830 && event.button.x < 880 && event.button.y > 390 && event.button.y < 440) return true;
	else return false;
}

int MouseOnHelp()
{
	if (event.button.x > 0 && event.button.x < 50 && event.button.y > 390 && event.button.y < 440) return true;
	else return false;
}
