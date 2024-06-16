#ifndef CHOOSELEVELSURFACE_H
#define CHOOSELEVELSURFACE_H
#include "Button.h"
#include "class.h"
#include <sstream> 
const int levelnumber=5;
class chooselevelsurface
{
		LTexture background;
		Button* levels[levelnumber];
	public:
		chooselevelsurface();
		~chooselevelsurface();
		void render();
		void handleEvent(SDL_Event& e,int& level);
};
chooselevelsurface::chooselevelsurface():background("./chooselevelsurface.png")
{
	stringstream levelnum;  
	
	for(int i=0;i<levelnumber;i++)
	{
		levelnum.str("");
		levelnum<<"./"<<i+1<<".png";
		levels[i]=new Button(levelnum.str());
		if(i<3)
		{
			levels[i]->setposition(125+150*i,150,100,100);
		}else
		{
			levels[i]->setposition(175+150*(i-3),300,100,100);
		}
	}
}

void chooselevelsurface::handleEvent(SDL_Event& e,int& level)
{
	for(int i=0;i<levelnumber;i++)
	{
		levels[i]->handleEvent(e);
		if(levels[i]->getpressed())
			level=i+1;
	}
}
void chooselevelsurface::render()
{
	SDL_Rect clip={0,0,background.getWidth(),background.getHeight()};
	SDL_Rect view={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	background.render(&clip,&view);
	for(int i=0;i<levelnumber;i++)
	{
		levels[i]->render();
	}
}
	
chooselevelsurface::~chooselevelsurface()
{
	for(int i=0;i<levelnumber;i++)
	{
		delete levels[i];
	}
}
#endif 
