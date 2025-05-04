#pragma once
#ifndef GAMERUN_H
#define GAMERUN_H
#include<vector>
#include "Core.h"
#include "Object.h"
class Gamerun {
public:
	Gamerun(SDL_Event g_event);
	void reload();
	double getCurrentFrame() { return currentFrame; }
	void HandleEvents();
	void pauseButton() { gamePause = !gamePause; }
	void homeButton() { state = 0; }
	void gameButton() { state = 1; }
	void buttonControler();
	void fPlayerControl(double deltaTime);
	void sPlayerControl(double deltaTime);
	void titleScreen();
	void loadMap();
	void drawMap();
	void gamePlay();
	//	void gameOver();
	void Control();
private:
	short** tileMap;
	Core gameCore;
	SDL_Event event;
	bool gamePause;
	bool gameRunning;
	Player firstPlayer;
	Player secondPlayer;
	std::vector<Boost> boost;
	std::vector<Tile> tile;
	SDL_Texture* mapTexture;
	std::vector<Button> gameplay;
	short state;
	Uint16 lastTick;
	Uint16 currentTick;
	const Uint8* keystates ;
	float currentFrame;
	double deltaTime;
	const int FRAME_RATE = 60;
	const int FRAME_DELAY = 1000 / FRAME_RATE;
};
#endif // !GAMERUN_H

