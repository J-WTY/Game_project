#ifndef SPRITE_H
#define SPRITE_H
#include "class.h"

class Sprite 
{
	protected:
		LTexture spriteTexture;
		SDL_Rect Position;
	public:
		Sprite();
		~Sprite();
		bool loadFromFile(std::string path);
		void render(SDL_Rect& camera, SDL_Rect* clip=NULL);
		const SDL_Rect* getPos();
};
extern SDL_Renderer* gRenderer;
Sprite::Sprite()
{
	
}

Sprite::~Sprite()
{
	spriteTexture.free();
}

bool Sprite::loadFromFile(std::string path)
{
	spriteTexture.loadFromFile(path);
	//cout <<"spriteTexture.mwidth" << spriteTexture.mWidth<<endl;
}

void Sprite::render(SDL_Rect& camera,SDL_Rect* clip)
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { Position.x-camera.x, Position.y-camera.y, Position.w, Position.h };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopy( gRenderer, spriteTexture.mTexture, clip, &renderQuad );
}

const SDL_Rect* Sprite::getPos()
{
	return &Position;
}
#endif
