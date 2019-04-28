#include "videopoker.h"

#ifndef MAIN_H
#define MAIN_H
	void DrawCard();
	void DrawToken();
	void DrawScore();
	void DrawText();
	void CheckEvent();
	int CheckCardCombo();
	void VIDEOPOKER_Init();
	void VIDEOPOKER_Quit();
	void VIDEOPOKER_Game();
	int MouseOnToken(Token object);
	int MouseOnCard(Card object);
	int MouseOnDeck();
	int MouseOnMute();
	int MouseOnHelp();
#endif
