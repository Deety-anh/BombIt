#pragma once
#ifndef CORE_H
#define CORE_H
#include<SDL2/SDL.h>
#include<SDL2/SDL_image.h>
#include<SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
#include<vector>
#include<iostream>

class Core {
public:
	Core(const char* title, int screenWidth, int screenHeight);
	void clear();
	void cleanup();
	void display();
	void renderTexture(int x, int y, SDL_Texture* texture);	
	SDL_Renderer* getRenderer() { return renderer; }
	SDL_Texture* loadTexture(const char* filePath);
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
};
#endif // !CORE_H