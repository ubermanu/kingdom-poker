#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>

#include "videopoker.h"

int i, j;

void BlendDeck(int *array)
{
    for (i = 0; i < 52; i++) array[i] = i;
    for (i = 0; i < 52; i++)
    {
        int tmp = array[i];
		int rnd = rand() % 52;
        array[i] = array[rnd];
        array[rnd] = tmp;
    }
}

void ApplySurface(int x, int y, SDL_Surface *source, SDL_Surface *destination, SDL_Rect *sprite)
{
	SDL_Rect offset;
	offset.x = x;
	offset.y = y;
	SDL_BlitSurface(source, sprite, destination, &offset);
}

int CheckCardCombo(Card *a_tmp)
{
    int n_tmp[5], c_tmp[5];
    int v_tmp;
    int result = 0;

    //Tri des cartes par numéro
    for (i = 0; i < 5; i++) n_tmp[i] = a_tmp[i].value % 13;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (n_tmp[j+1] < n_tmp[j])
            {
                v_tmp = n_tmp[j+1];
                n_tmp[j+1] = n_tmp[j];
                n_tmp[j] = v_tmp;
            }
        }
    }
    //Détection des Paires(1), Double Paires(2), Brelans(3), Carrés(7) et Full(6)
    for (i = 0; i < 4; i++)
    {
        if (result == 1)
        {
            if (n_tmp[i] == n_tmp[i+2]) return 6; //full
            else if (n_tmp[i] == n_tmp[i+1]) return 2; //double paire
        }
        else if (result == 3)
        {
            if (n_tmp[i] == n_tmp[i+1]) return 6; //full
        }
        else
        {
            if (n_tmp[i] == n_tmp[i+3]) return 9; //carré
            if (n_tmp[i] == n_tmp[i+2]) //il y a brelan
            {
                result = 3;
                i += 2; //on saute les etapes
            }
            else if (n_tmp[i] == n_tmp[i+1]) // il y a paire
            {
                result = 1;
                i += 1; //on saute les etapes
            }
        }
    }

    //Détection des Quintes(4)
    for (i = 0; i < 4; i++)
    {
        if (n_tmp[i] != n_tmp[i+1] - 1) break;
        if (i == 3) result = 4;
    }

    //Détection d'une éventuelle Quinte Flush Royale
    if (n_tmp[0] == 0 && n_tmp[1] == 9 && n_tmp[2] == 10 && n_tmp[3] == 11 && n_tmp[4] == 12) result = 40;
    //Tri des cartes par couleur
    for (i = 0; i < 5; i++) c_tmp[i] = a_tmp[i].value / 13;
    for (i = 0; i < 5; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (c_tmp[j+1] < c_tmp[j])
            {
                v_tmp = c_tmp[j+1];
                c_tmp[j+1] = c_tmp[j];
                c_tmp[j] = v_tmp;
            }
        }
    }
    //Détection d'une Flush(5), Quinte Flush(8) voire Quinte Flush Royale(9)
    if (c_tmp[0] == c_tmp[4])
    {
        switch (result)
        {
            case 4:
                result = 11; //quinte flush
                break;
            case 40:
                result = 21; //quinte flush royale
                break;
            default:
                result = 5; //flush
        }
    }
    if (result == 40) result = 4; //Si la Quinte Flush Royale n'as pas été validée, c'est que ce n'est qu'une suite
    return result;
}
