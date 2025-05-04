#include "Gamerun.h"

void Gamerun::titleScreen() {
	reload();
	loadMap();
	state = 1;
	Control();
}
void Gamerun::buttonControler() {
	switch (state) {
	case 0:
		break;
	case 1:
		int x, y;
		SDL_GetMouseState(&x, &y);
		for (Button b : gameplay) {
			if (b.inSide(x, y)) {
				switch (b.getState()) {
				case 0:
					pauseButton();
					break;
				}
			}
		}
	}
}
void Gamerun::gamePlay() {
	std::cout << "DONE!";
	while (gameRunning) {
		HandleEvents();
		gameCore.clear();
		drawMap();
		if (!gamePause) {
			firstPlayer.update(deltaTime, tileMap, &gameCore);
			secondPlayer.update(deltaTime, tileMap, &gameCore);
			for (short i = 0; i < boost.size(); i++) {
				Position player1Pos = Position(int(firstPlayer.getPosition().x + 24) / 48, int(firstPlayer.getPosition().y + 24) / 48);
				Position player2Pos = Position(int(secondPlayer.getPosition().x + 24) / 48, int(secondPlayer.getPosition().y + 24) / 48);
				Position boostPos = Position(int(boost[i].getPosition().x + 24) / 48, int(boost[i].getPosition().y + 24) / 48);
				if (player1Pos.x == boostPos.x && player1Pos.y == boostPos.y) {
					boost[i].setBoost(firstPlayer.getBoost(boost[i].getType()));
					boost.erase(boost.begin() + i);
				}
				else if (player2Pos.x == boostPos.x && player2Pos.y == boostPos.y) {
					boost[i].setBoost(secondPlayer.getBoost(boost[i].getType()));
					boost.erase(boost.begin() + i);
				}
			}
			lastTick = currentTick;
			currentTick = SDL_GetTicks();
			deltaTime = (currentTick - lastTick) / 1000.0; // Convert to seconds
			currentFrame += 0.3;
			short div = currentFrame / 1000;
			currentFrame -= div * 1000; // Reset frame count
			fPlayerControl(deltaTime);
			sPlayerControl(deltaTime);
			if (!firstPlayer.canPlace() && firstPlayer.getBomb()->getBombTime() > 0) {
				firstPlayer.getBomb()->render(gameCore.getRenderer());
			}
			if (!secondPlayer.canPlace() && secondPlayer.getBomb()->getBombTime() > 0) {
				secondPlayer.getBomb()->render(gameCore.getRenderer());
			}
		}
		if (firstPlayer.getheal() == 0 || secondPlayer.getheal() == 0) {
			if (firstPlayer.getheal() == 0) { std::cout << "Player 2 wins!" << std::endl; }
			else { std::cout << "Player 1 wins!" << std::endl; }
			gameRunning = false;
		}
		firstPlayer.renderPlayer(currentFrame, gameCore.getRenderer());
		secondPlayer.renderPlayer(currentFrame, gameCore.getRenderer());
		for (Button b : gameplay) { b.render(gameCore.getRenderer()); }
		gameCore.display();
		SDL_Delay(FRAME_DELAY);
	}
	state = 3;
}
void Gamerun::Control() {
	switch (state) {
	case 0:
		titleScreen();
		break;
	case 1:
		gamePlay();
		break;
	case 2:
		//	gameOver();
		break;
	default:
		gameCore.cleanup();
		break;
	}
}