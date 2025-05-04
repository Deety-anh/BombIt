
#include "Gamerun.h"

SDL_Event g_event;
Gamerun game(g_event);
int main(int argc, char* argv[]) {
	game.Control();
	return 0;                                 
}