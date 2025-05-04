#include "Gamerun.h"
#include <random>

Gamerun::Gamerun(SDL_Event g_event) : gameCore("Bomb It", 480, 720),
firstPlayer(Position(0, 0), Bomb(Position(0, 0), gameCore.loadTexture("assets/bomb/bomb.png"), gameCore.loadTexture("assets/bomb/explosion.png")), gameCore.loadTexture("assets/player/1/D_Idle.png"), gameCore.loadTexture("assets/player/1/U_Idle.png"), gameCore.loadTexture("assets/player/1/S_Idle.png"), gameCore.loadTexture("assets/player/1/D_Walk.png"), gameCore.loadTexture("assets/player/1/U_Walk.png"), gameCore.loadTexture("assets/player/1/S_Walk.png")),
secondPlayer(Position(0, 0), Bomb(Position(0, 0), gameCore.loadTexture("assets/bomb/bomb.png"), gameCore.loadTexture("assets/bomb/explosion.png")), gameCore.loadTexture("assets/player/2/D_Idle.png"), gameCore.loadTexture("assets/player/2/U_Idle.png"), gameCore.loadTexture("assets/player/2/S_Idle.png"), gameCore.loadTexture("assets/player/2/D_Walk.png"), gameCore.loadTexture("assets/player/2/U_Walk.png"), gameCore.loadTexture("assets/player/2/S_Walk.png")),
mapTexture(gameCore.loadTexture("assets/map.png")),
gameRunning(true),
state(0),
event(g_event),
keystates(SDL_GetKeyboardState(NULL)),
lastTick(0),
currentTick(SDL_GetTicks()),
currentFrame(0),
deltaTime(0.0),
gamePause(false)
	// Initialize the tile map with some values (0 for empty, 1 for wall, 2 for breakable wall)
	// This is just an example; you can modify it as needed.
	// The tileMap should be initialized in a way that matches your game logic.
{
	Button b(Position(0, 0), gameCore.loadTexture("assets/button/PauseButton.png"),0);
	gameplay.push_back(b);
}
void Gamerun::reload() {
	short tile[15][10] = {
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 1, 1, 1, 1, 2, 1, 1, 1, 1 },
		{ 0, 1, 0, 0, 0, 2, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 0, 1, 0, 0, 0, 1, 0, 0, 0, 1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 1, 0, 0, 0, 1, 0 ,1 },
		{ 1, 0, 0, 1, 0, 0, 0, 1, 0 ,1 },
		{ 1, 0, 0, 1, 0, 0, 0, 1, 0 ,1 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 0, 0, 0, 1, 0, 0, 0, 1, 0 },
		{ 1, 1, 1, 1, 1, 1, 1, 1, 1, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }
	};
	tileMap = new short* [15];
	for (int i = 0; i < 15; i++) {
		tileMap[i] = new short[10];
		for (int j = 0; j < 10; j++) {
			tileMap[i][j] = tile[i][j];
		}
	}
	firstPlayer.setPosition(48, 48);
	secondPlayer.setPosition(396, 624);
}
void Gamerun::fPlayerControl(double deltaTime) {
	if (keystates[SDL_SCANCODE_SPACE] && firstPlayer.canPlace()) {
		firstPlayer.setCanPlaceBomb(false);
	}
	if (keystates[SDL_SCANCODE_UP]) {
		firstPlayer.setDirection(0);
		firstPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_DOWN]) {
		firstPlayer.setDirection(1);
		firstPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_LEFT]) {
		firstPlayer.setDirection(2);
		firstPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_RIGHT]) {
		firstPlayer.setDirection(3);
		firstPlayer.move(deltaTime, tileMap);
	}
}
void Gamerun::sPlayerControl(double deltaTime) {
	if (keystates[SDL_SCANCODE_J] && secondPlayer.canPlace()) {
		secondPlayer.setCanPlaceBomb(false);
	}
	if (keystates[SDL_SCANCODE_W]) {
		secondPlayer.setDirection(0);
		secondPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_S]) {
		secondPlayer.setDirection(1);
		secondPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_A]) {
		secondPlayer.setDirection(2);
		secondPlayer.move(deltaTime, tileMap);
	}
	else if (keystates[SDL_SCANCODE_D]) {
		secondPlayer.setDirection(3);
		secondPlayer.move(deltaTime, tileMap);
	}
}
void Gamerun::HandleEvents() {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			gameRunning = false;
		}
		if (event.type == SDL_KEYDOWN) {
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				gameRunning = false;
				break;
			case SDLK_p:
				gamePause = !gamePause;
				break;
			}
		}
		if (event.type == SDL_MOUSEBUTTONDOWN) { buttonControler(); }
	}
}
void Gamerun::loadMap() {
	// Load the map texture
	mapTexture = gameCore.loadTexture("assets/map.png");
	SDL_Texture* speedBoostTexture = gameCore.loadTexture("assets/boosts/speedboost.png");
	SDL_Texture* ironBodyTexture = gameCore.loadTexture("assets/boosts/ironbody.png");
	SDL_Texture* bombBoostTexture = gameCore.loadTexture("assets/boosts/bombboost.png");
	for (short i = 0; i < 15; i++) {
		for (short j = 0; j < 10; j++) {
			if (tileMap[i][j] == 2) {
				std::random_device rd;
				std::mt19937 gen(rd());
				short boostType = gen() % 3;
				Boost b(Position(j * 48 , i * 48), speedBoostTexture, boostType);
				switch (boostType) {
				case 0:
					b = Boost(Position(j * 48, i * 48), speedBoostTexture, boostType);
					b.setPosition(j * 48 + 6, i * 48 + 6);
					boost.push_back(b);
					break;
				case 1:
					b = Boost(Position(j * 48, i * 48), ironBodyTexture, boostType);
					b.setPosition(j * 48 + 6, i * 48 + 6);
					boost.push_back(b);
					break;
				case 2:
					b = Boost(Position(j * 48, i * 48), bombBoostTexture, boostType);
					b.setPosition(j * 48 + 6, i * 48 + 6);
					boost.push_back(b);
					break;
				}
			}
		}
	}
}
void Gamerun::drawMap() {
	gameCore.renderTexture(0, 0, mapTexture);
	for (Boost b : boost) { b.render(gameCore.getRenderer());
	}
	for (short i = 0; i < 15; i++) {
		for (short j = 0; j < 10; j++) {
			if (tileMap[i][j] == 2) {
				SDL_Texture* tileTexture = gameCore.loadTexture("assets/Tile/tile.png");
				gameCore.renderTexture(j * 48 + 6, i * 48 + 6, tileTexture);
			}
		}
	}
}
