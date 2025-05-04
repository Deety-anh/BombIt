#include "Object.h"
#include <iostream>
Player::Player(Position m_pos, Bomb m_bomb, SDL_Texture* D_IdleTexture, SDL_Texture* U_IdleTexture, SDL_Texture* S_IdleTexture,SDL_Texture*D_WalkTexture, SDL_Texture* U_WalkTexture, SDL_Texture* S_WalkTexture) :
	Object(m_pos),
	canPlaceBomb(true),
	speed(250),
	speedBoostTime(0),
	ironBodyTime(0),
	bombBoostTime(0),
	bomb(m_bomb),
	heal(3),
	mode(0),// Stand mode
	dir(0), // 0 = up
	D_Idle(D_IdleTexture),
	U_Idle(U_IdleTexture),
	S_Idle(S_IdleTexture),
	D_Walk(D_WalkTexture),
	S_Walk(S_WalkTexture),
	U_Walk(U_WalkTexture)

{}
double* Player::getBoost(short type) {
	switch (type) {
	case 0: // Speed boost
		return &speedBoostTime;
	case 1: // Iron body
		return &ironBodyTime;
	case 2: // Bomb boost
		return &bombBoostTime;
	default:
		return nullptr;
	}
}
void Player::fixPosition(short dir) {
	Position center(getPosition().x + 24, getPosition().y + 24);
	short centerX = (int)(center.x / 48);
	short centerY = (int)(center.y / 48);
	Position standardCenter(centerX * 48 + 24, centerY * 48 + 24);
	switch (dir) {
	case 0:
		if (abs(int(center.x - standardCenter.x)) <= 11) {
			center.x = standardCenter.x;
		}
		break;
	case 1:
		if (abs(int(center.y - standardCenter.y)) <= 11) {
			center.y = standardCenter.y;
		}
		break;
	}
	setPosition(center.x - 24, center.y - 24);
}
void Player::move(double deltaTime, short**tilemap) {
	Position newPos;
	mode = 1;
	int s = speed;
	if (speedBoostTime > 0) { s *= 1.5; }
	switch (dir) {
	case 0: // Up
		// Check for collision with the map
		fixPosition(0);
		newPos = Position(getPosition().x, getPosition().y - s * deltaTime);
		if (newPos.y < 0) { newPos.y = 0; }
		else {
			short nomalizeY = (int)(newPos.y / 48);
			short nomalizeX = (int)(newPos.x / 48);
			short nomalizeX2 = (int)((newPos.x + 47.99) / 48);
			if ((tilemap[nomalizeY][nomalizeX] != 1 && tilemap[nomalizeY][nomalizeX] != 3) || (tilemap[nomalizeY][nomalizeX2] != 1 && tilemap[nomalizeY][nomalizeX2] != 3)) {
				newPos.y = (nomalizeY + 1) * 48;
			}
		}
		setPosition(newPos.x, newPos.y);
		break;
	case 1: // Down
		// Check for collision with the map
		fixPosition(0);
		newPos = Position(getPosition().x, getPosition().y + s * deltaTime);
		if (newPos.y > 720 - 48) { newPos.y = 720 - 48; }
		else {
			short nomalizeY = (int)((newPos.y + 47.99) / 48);
			short nomalizeX = (int)(newPos.x / 48);
			short nomalizeX2 = (int)((newPos.x + 47.99) / 48);
			if ((tilemap[nomalizeY][nomalizeX] != 1 && tilemap[nomalizeY][nomalizeX] != 3) || (tilemap[nomalizeY][nomalizeX2] != 1 && tilemap[nomalizeY][nomalizeX2] != 3)) {
				newPos.y = (nomalizeY - 1) * 48;
			}
		}
		setPosition(newPos.x, newPos.y);
		break;
	case 2: // Left
		// Check for collision with the map
		fixPosition(1);
		newPos = Position(getPosition().x - s * deltaTime, getPosition().y);
		if (newPos.x < 0) { newPos.x = 0; }
		else {
			short nomalizeX = (int)(newPos.x / 48);
			short nomalizeY = (int)(newPos.y / 48);
			short nomalizeY2 = (int)((newPos.y + 47.99) / 48);
			if ((tilemap[nomalizeY][nomalizeX] != 1 && tilemap[nomalizeY][nomalizeX] != 3) || (tilemap[nomalizeY2][nomalizeX] != 1 && tilemap[nomalizeY2][nomalizeX] != 3)) {
				newPos.x = (nomalizeX + 1) * 48;
			}
		}
		setPosition(newPos.x, newPos.y);
		break;
	case 3: // Right
		// Check for collision with the map
		fixPosition(1);
		newPos = Position(getPosition().x + s * deltaTime, getPosition().y);
		if (newPos.x > 480 - 48) { newPos.x = 480 - 48; }
		else {
			short nomalizeX = (int)((newPos.x + 47.99) / 48);
			short nomalizeY = (int)(newPos.y / 48);
			short nomalizeY2 = (int)((newPos.y + 47.99) / 48);
			if ((tilemap[nomalizeY][nomalizeX] != 1 && tilemap[nomalizeY][nomalizeX] != 3) || (tilemap[nomalizeY2][nomalizeX] != 1 && tilemap[nomalizeY2][nomalizeX] != 3)) {
				newPos.x = (nomalizeX - 1) * 48;
			}
		}
		setPosition(newPos.x, newPos.y);
		break;
		}
}
void Player::update(double deltaTime, short** tilemap, Core* core) {
	mode = 0; // Stand mode
	int normalizeX = (int)((getPosition().x + 24) / 48);
	int normalizeY = (int)((getPosition().y + 24) / 48);
	if (tilemap[normalizeY][normalizeX] == 3 && ironBodyTime <= 0) {
		heal--;
		ironBodyTime = 5;
	}
	if (canPlaceBomb) {
		Position center(getPosition().x + 24, getPosition().y + 24);
		short centerX = (int)(center.x / 48);
		short centerY = (int)(center.y / 48);
		bomb.setPosition(centerX * 48, centerY * 48);
		bomb.setBombTime(BOMB_TIME);
		bomb.setExplosionTime(EXPLOSION_TIME);
	}
	else {
		bomb.setBombTime(bomb.getBombTime() - deltaTime);
		bomb.setScale(1.0 - (bomb.getBombTime() / BOMB_TIME));
	}
	if (speedBoostTime > 0) {
		speedBoostTime -= deltaTime;
	}
	if (ironBodyTime > 0) {
		ironBodyTime -= deltaTime;
	}
	if (bombBoostTime > 0) {
		bombBoostTime -= deltaTime;
	}
	if (bomb.getBombTime() <= 0) {
		bomb.setExplosionTime(bomb.getExplosionTime() - deltaTime);
		short centerX = (int)(bomb.getPosition().x / 48);
		short centerY = (int)(bomb.getPosition().y / 48);
		if (bomb.getExplosionTime() < 0) { 
			canPlaceBomb = true; 
			tilemap[centerY][centerX] = 1;
			if(bombBoostTime > 0){
				for (short i = centerX - 1; i >= 0 && tilemap[centerY][i]; i--) {
					if (tilemap[centerY][i] == 2) { tilemap[centerY][i] = 1; break; }
					else { tilemap[centerY][i] = 1; }
				}
				for (short i = centerX + 1; i < 10 && tilemap[centerY][i]; i++) {
					if (tilemap[centerY][i] == 2) { tilemap[centerY][i] = 1; break; }
					else { tilemap[centerY][i] = 1; }
				}
				for (short i = centerY - 1; i >= 0 && tilemap[i][centerX]; i--) {
					if (tilemap[i][centerX] == 2) { tilemap[i][centerX] = 1; break; }
					else { tilemap[i][centerX] = 1; }
				}
				for (short i = centerY + 1; i < 15 && tilemap[i][centerX]; i++) {
					if (tilemap[i][centerX] == 2) { tilemap[i][centerX] = 1; break; }
					else { tilemap[i][centerX] = 1; }
				}
			}
			else {
				if (centerX > 0 && tilemap[centerY][centerX - 1]) { tilemap[centerY][centerX - 1] = 1; }
				if (centerX < 10 && tilemap[centerY][centerX + 1]) { tilemap[centerY][centerX + 1] = 1; }
				if (centerY > 0 && tilemap[centerY - 1][centerX]) { tilemap[centerY - 1][centerX] = 1; }
				if (centerY < 15 && tilemap[centerY + 1][centerX]) { tilemap[centerY + 1][centerX] = 1; }
			}
		}
		else {
			tilemap[centerY][centerX] = 3;
			core->renderTexture(centerX * 48, centerY * 48, bomb.getExplosionTexture());
			if (bombBoostTime > 0) {
				for (short i = centerX - 1; i >= 0 && tilemap[centerY][i] != 0 && tilemap[centerY][i] != 2; i--) {
					tilemap[centerY][i] = 3;
					core->renderTexture(i * 48, centerY * 48, bomb.getExplosionTexture());
				}
				for (short i = centerX + 1; i < 10 && tilemap[centerY][i] != 0 && tilemap[centerY][i] != 2; i++) {
					tilemap[centerY][i] = 3;
					core->renderTexture(i * 48, centerY * 48, bomb.getExplosionTexture());
				}
				for (short i = centerY - 1; i >= 0 && tilemap[i][centerX] != 0 && tilemap[i][centerX] != 2; i--) {
					tilemap[i][centerX] = 3;
					core->renderTexture(centerX * 48, i * 48, bomb.getExplosionTexture());
				}
				for (short i = centerY + 1; i < 15 && tilemap[i][centerX] != 0 && tilemap[i][centerX] != 2; i++) {
					tilemap[i][centerX] = 3;
					core->renderTexture(centerX * 48, i * 48, bomb.getExplosionTexture());
				}
			}
			else {
				if (centerX > 0 && tilemap[centerY][centerX - 1] != 0) {
					tilemap[centerY][centerX - 1] = 3;
			 		core->renderTexture((centerX - 1) * 48, centerY * 48, bomb.getExplosionTexture());
				}
				if (centerX < 9 && tilemap[centerY][centerX + 1] != 0) {
					tilemap[centerY][centerX + 1] = 3;
					core->renderTexture((centerX + 1) * 48, centerY * 48, bomb.getExplosionTexture());
				}
				if (centerY > 0 && tilemap[centerY - 1][centerX] != 0) {
					tilemap[centerY - 1][centerX] = 3;
					core->renderTexture(centerX * 48, (centerY - 1) * 48, bomb.getExplosionTexture());
				}
				if (centerY < 14 && tilemap[centerY + 1][centerX] != 0) {
					tilemap[centerY + 1][centerX] = 3;
					core->renderTexture(centerX * 48, (centerY + 1) * 48, bomb.getExplosionTexture());
				}
			}
		}
	}
}
void Player::renderPlayer(short currentFrame, SDL_Renderer* renderer) {
	int frame;
	switch (mode) {
	case 0: // Stand mode
		frame = currentFrame % 4; // 4 frames for animation
		if (dir == 0) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, U_Idle, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 1) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, D_Idle, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 2) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, S_Idle, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 3) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, S_Idle, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		break;
	case 1: // Move mode
		frame =  currentFrame % 6; // 6 frames for animation
		if (dir == 0) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, U_Walk, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 1) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, D_Walk, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 2) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, S_Walk, &src, &dst, 0, NULL, SDL_FLIP_NONE);
		}
		else if (dir == 3) {
			SDL_Rect src;
			src.x = 48 * frame;
			src.y = 0;
			src.w = 48;
			src.h = 48;
			SDL_Rect dst;
			dst.x = getPosition().x;
			dst.y = getPosition().y;
			dst.w = 48;
			dst.h = 48;
			SDL_RenderCopyEx(renderer, S_Walk, &src, &dst, 0, NULL, SDL_FLIP_HORIZONTAL);
		}
		break;
	}
}