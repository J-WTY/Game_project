#ifndef ITEM_H
#define ITEM_H
#include "class.h"

class Item
{ 
	LTexture ItemTexture;
	SDL_Rect Position;
	bool got;
public:
	Item(int x, int y);
	~Item();
	void reset();
	bool loadFromFile(std::string path);
	void gotten(bool);
	void render(SDL_Rect& camera);
	SDL_Rect getPos();
	
};

extern SDL_Renderer* gRenderer;
extern bool checkCollision(SDL_Rect, SDL_Rect);
Item::Item(int x, int y)
{
	Position.x = x;
	Position.y = y;
	Position.w = 40;
	Position.h = 40;
	ItemTexture.loadFromFile("./book.png");
	reset();
}


Item::~Item()
{
	ItemTexture.free();
}

void Item::reset()
{
	got = false;
}

bool Item::loadFromFile(std::string path)
{
	ItemTexture.loadFromFile(path);
}

void Item::gotten(bool flag)
{
	got = flag;
}

void Item::render(SDL_Rect& camera)
{
	if(got)
		return;
	if(checkCollision(camera,Position))
	{
		SDL_Rect view={Position.x-camera.x,Position.y-camera.y,Position.w,Position.h};
		SDL_Rect clip={0,0,ItemTexture.getWidth(),ItemTexture.getHeight()};
		ItemTexture.render(&clip,&view);
	}
}

SDL_Rect Item::getPos()
{
	return Position;
}
#endif
