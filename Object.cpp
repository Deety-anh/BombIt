#include "Object.h"

Object::Object(Position m_pos, SDL_Texture* tex) : texture(tex), scale(1),dir(SDL_FLIP_NONE) {
	rect.x = 0;
	rect.y = 0;
	rect.w = 48;
	rect.h = 48;
	SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h); 
}
Object::Object(Position m_pos) : pos(m_pos), scale(1), dir(SDL_FLIP_NONE) {
	rect.x = 0;
	rect.y = 0;
	rect.w = 48;
	rect.h = 48;
}
void Object::setPosition(float x, float y) {
	pos.x = x;
	pos.y = y;
}
void Object::render(SDL_Renderer* renderer) {
	SDL_Rect src;
	src.x = getRect().x;
	src.y = getRect().y;
	src.w = getRect().w;
	src.h = getRect().h;
	SDL_Rect dst;
	dst.x = getPosition().x + ((getRect().w * (1 - scale)) / 2);
	dst.y = getPosition().y + ((getRect().h * (1 - scale)) / 2);
	dst.w = getRect().w * scale;
	dst.h = getRect().h * scale;
	SDL_RenderCopyEx(renderer, texture, &src, &dst, 0, 0, dir);
}

