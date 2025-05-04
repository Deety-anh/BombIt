#include "Core.h"

Core::Core(const char* title, int screenWidth, int screenHeight) {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) { std::cout << "OOPS! SDL HAS FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; }
	else {
		window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if (window == NULL) { std::cout << "OOPS! WINDOW HAS FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; }
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
		if (renderer == NULL) { std::cout << "OOPS! RENDERER HAS FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; }
	}
}
void Core::cleanup() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Core::renderTexture(int x, int y, SDL_Texture* texture) {
	SDL_Rect src;
	src.x = 0;
	src.y = 0;
	SDL_QueryTexture(texture, NULL, NULL, &src.w, &src.h);
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = src.w;
	dst.h = src.h;
	SDL_RenderCopy(renderer, texture, &src, &dst);
}
void Core::clear() { SDL_RenderClear(renderer); }
void Core::display() { SDL_RenderPresent(renderer); }
SDL_Texture* Core::loadTexture(const char* filePath) {
	SDL_Texture* texture = IMG_LoadTexture(renderer, filePath);
	if (texture == NULL) { std::cout << "OOPS! TEXTURE HAS FAILED TO INIT. ERROR: " << SDL_GetError() << std::endl; }
	return texture;
}