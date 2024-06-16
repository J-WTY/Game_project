#ifndef NORMALSURFACE_H
#define NORMALSURFACE_H
#include "class.h"
#include "Button.h"
class Normalsurface
{	
		LTexture background;
		TTexture label;
		Button start; 
	public:
		Normalsurface(string ground_path ,string text_content,string font_path,SDL_Color color,int size);
		~Normalsurface();
		void handleevent(int &level,SDL_Event& e);
		void render(int x1,int y1,int x2,int y2);
	
};

//Scene sprites
Normalsurface::Normalsurface(string ground_path ,string text_content,string font_path,SDL_Color color,int size):start("./play.png")
{
	if(!background.loadFromFile(ground_path))
	{
		cout<<"Failed to load file! "<<ground_path<<endl;
	}
	if(!label.setFont(font_path,size))
	{
		cout<<"Failed to load file "<<font_path<<endl;
	}
	else
	{
		if(!label.loadFromRenderText(text_content,color))
		{
			cout<<"Failed to load text "<<endl;
		}
	}
	start.setposition((SCREEN_WIDTH-150)/2,(SCREEN_HEIGHT-75)/2+100,150,75);
	return;
}

void Normalsurface::render(int x1,int y1,int x2,int y2)
{
	SDL_Rect wholeview={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
	SDL_Rect wholeclip={0,0,background.getWidth(),background.getHeight()};
	background.render(&wholeclip,&wholeview);
	if(gRenderer==NULL)
		cout<<"Failed to render background!";
//	label.render((SCREEN_WIDTH-label.getWidth())/2,(SCREEN_HEIGHT-label.getHeight())/2);
//	SDL_Rect view={(SCREEN_WIDTH-150)/2,(SCREEN_HEIGHT-100)/2,150,100};
	start.render();
}

Normalsurface::~Normalsurface()
{
	background.free();
	label.free();
}
void Normalsurface::handleevent(int &level,SDL_Event& e)
{
	/*
	if(e.key.keysym.sym==SDLK_SPACE)
		level=1;
		*/
	start.handleEvent(e);
	if(start.getpressed())
		level=-1;
	return;
}
#endif
