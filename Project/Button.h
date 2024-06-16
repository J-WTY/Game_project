#ifndef BUTTON_H
#define BUTTON_H
#include "class.h"
class Button
{  	
	public:
		Button(string path);
		void handleEvent(SDL_Event& e);
		void render();
		bool getpressed(){return pressed;}
		void setposition(int x,int y,int w,int h);
	private: 
		LTexture Buttontexture;
		SDL_Rect mBox; 
		bool pressed;
};
Button::Button(string path)
{
	mBox.x=0;
	mBox.y=0;
	if(Buttontexture.mTexture==NULL)
	{
		Buttontexture.loadFromFile(path);
	}
	mBox.w=0;
	mBox.h=0; 
	pressed=false;
}
void Button::setposition(int x,int y,int w,int h)
{
	mBox.x=x;
	mBox.y=y;
	mBox.w=w;
	mBox.h=h;
	return;
}
void Button::handleEvent(SDL_Event& e)
{
	if(e.type==SDL_MOUSEMOTION||e.type==SDL_MOUSEBUTTONDOWN)
	{
		int x,y;
		SDL_GetMouseState(&x,&y);
	//	cout<<"x :"<<x<<" y: "<<y<<endl;
		if((x-mBox.x)>=0&&(x-mBox.x)<=mBox.w&&y>=mBox.y&&(y-mBox.y)<=mBox.h)
		{
		//	cout<<"jj"<<endl;
			if(e.type==SDL_MOUSEBUTTONDOWN)
			{
				pressed=true;
				return;
			}
		}	
	}
	pressed=false;
}
void Button::render()
{
	SDL_Rect clip={0,0,Buttontexture.getWidth(),Buttontexture.getHeight()};
	Buttontexture.render(&clip,&mBox);
}
#endif
