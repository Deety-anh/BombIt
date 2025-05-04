#pragma once
#ifndef OBJECT_H
#define OBJECT_H
#include<vector>
#include"Core.h"

struct Position {
	float x;
	float y;
	Position() : x(0.0f), y(0.0f) {}
	Position(float m_x, float m_y) : x(m_x), y(m_y) {}
};
class Object {
public:
	Object(Position m_pos, SDL_Texture* m_tex);
	Object(Position m_pos);
	Position getPosition() { return pos; }
	void setPosition(float x, float y);
	void setScale(double s) { scale = s; }
	void setDirection(SDL_RendererFlip d) { dir = d; }
	double getScale() { return scale; }
	SDL_Texture* getTexture() { return texture; }
	void render(SDL_Renderer* renderer);
	SDL_Rect& getRect() { return rect; }
private:
	SDL_RendererFlip dir;
	Position pos;
	double scale;
	SDL_Texture* texture;
	SDL_Rect rect;
};
class Boost : public Object {
public:
	Boost(Position m_pos, SDL_Texture* tex, short type) : Object(m_pos, tex), boostType(type) {}
	void setBoost(double* boostType) { *boostType = 6; }
	short getType() { return boostType; }
private:
	short boostType; // 0 = speed; 1 = iron body; 2 = bomb boost
};
class Tile : public Object {
public:
	Tile(Position m_pos, SDL_Texture* tex) : Object(m_pos, tex) {}
};
class Bomb : public Object {
public:
	Bomb(Position m_pos, SDL_Texture* tex,SDL_Texture* explosiontex) : Object(m_pos, tex), bombTime(1), explosionTime(0.5),explosionTexture(explosiontex) {}
	SDL_Texture* getExplosionTexture() { return explosionTexture; }
	void setBombTime(double t) { bombTime = t; }
	double getBombTime() { return bombTime; }
	void setExplosionTime(double t) { explosionTime = t; }
	double getExplosionTime() { return explosionTime; }
private:
	SDL_Texture* explosionTexture;
	double bombTime;
	double explosionTime;
};
class Player : public Object {
public:
	Player(Position m_pos, Bomb m_bomb, SDL_Texture* D_IdleTexture, SDL_Texture* U_IdleTexture, SDL_Texture* S_IdleTexture,SDL_Texture*D_WalkTexture, SDL_Texture* U_WalkTexture, SDL_Texture* S_WalkTexture);
	bool canPlace() { return canPlaceBomb; }
	Bomb* getBomb() { return &bomb; }
	int getheal() { return heal; }
	double* getBoost(short type);
	void setCanPlaceBomb(bool b) { canPlaceBomb = b; }
	void setSpeed(float s) { speed = s; }
	void fixPosition(short dir);
	void setDirection(short d) { dir = d; }
	void move(double deltaTime, short**tilemap);
	void update(double deltaTime, short** tilemap, Core* core);
	void renderPlayer(short currentFrame, SDL_Renderer* renderer);
private:
	SDL_Texture* D_Idle;
	SDL_Texture* U_Idle;
	SDL_Texture* S_Idle;
	SDL_Texture* D_Walk;
	SDL_Texture* U_Walk;
	SDL_Texture* S_Walk;
	short dir;
	short mode;// 0 = stand; 1 = move
	const double BOOST_TIME = 1.5;
	const double BOMB_TIME = 1;	
	const double EXPLOSION_TIME = 0.5;
	Bomb bomb;
	bool canPlaceBomb;
	float speed;
	int heal;
	double speedBoostTime;
	double ironBodyTime;
	double bombBoostTime;
};
class Button : public Object {
public:
	Button(Position pos, SDL_Texture* tex, short state) :Object(pos, tex) {
		rect.x = pos.x;
		rect.y = pos.y;
		SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
	}
	bool inSide(int x, int y) {
		return x >= rect.x && y >= rect.y && x <= (rect.x + rect.w) && y <= (rect.y + rect.h);
	}
	short getState() { return state; }
private:
	SDL_Rect rect;
	short state;
};
#endif // !OBJECT_H