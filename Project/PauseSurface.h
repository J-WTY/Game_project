#ifndef PAUSESURFACE_H
#define PAUSESURFACE_H
#include "class.h"
#include <sstream>
class PauseSurface
{
		LTexture background;
		TTexture timetexture;
		Timer clock;
		Button Home;
		Button again;
	public:
		PauseSurface();
		~PauseSurface(){};
		void render(bool& pause);
		void handleEvent(SDL_Event& e,int& level,bool& pause);
};

PauseSurface::PauseSurface() :background("./PauseSurface.png"),Home("./Home.png"),again("./again.png")
{	
	Home.setposition((SCREEN_WIDTH-200)/2+50,(SCREEN_HEIGHT-200)/2+140,50,50);
}
void PauseSurface::handleEvent(SDL_Event& e,int& level,bool& pause)
{
	Home.handleEvent(e);
	again.handleEvent(e);
	if(Home.getpressed())
	{
		level=0;
		pause=false;
	}
	if(pause&&clock.getTicks()==0)
	{
		clock.start();		  
	}/*
	if(pause&&clock.getTicks()>=5000)
	{
		pause=false;
	}*/
	if(!pause)
	{
		clock.stop();	
	}		
}
void PauseSurface::render(bool& pause)
{
	SDL_Rect clip={0,0,background.getWidth(),background.getHeight()};
	SDL_Rect view={(SCREEN_WIDTH-200)/2,(SCREEN_HEIGHT-200)/2,200,200};
	background.render(&clip,&view);
	//Continue.render();
	stringstream timetext;
	timetext.str("");
	if(pause&&clock.getTicks()>=3000)
	{
		pause=false;
		clock.stop();
	}
	if(clock.isStarted())
	{
		timetext<<3-(clock.getTicks()/1000);  
		timetexture.setFont("./orange juice 2.0.ttf",40);
		SDL_Color black={0,0,0,255};
		timetexture.loadFromRenderText(timetext.str().c_str(),black);
		timetexture.render((SCREEN_WIDTH-200)/2+80,(SCREEN_HEIGHT-200)/2+100);
	}
	
	Home.render();
//	again.render();
	/*	timecounter.setFont("./orange juice 2.0.ttf",40);
	timecounter.loadFromRenderText(timetext.str().c_str(),white);
	timecounter.render(SCREEN_WIDTH-timecounter.getWidth()-20,20);
	*/
}
#endif
