#ifndef CLASS1_H
#define CLASS1_H
//Using SDL, SDL_image, standard math, and strings
#include <SDL.h>
#include <SDL2/SDL_image.h>   
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <iostream>  

using namespace std;
//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
//The window renderer  
 SDL_Renderer* gRenderer=NULL ; 
//Texture wrapper class
class LTexture
{
		int mWidth;
		int mHeight;
	public:
		//Initializes variables
		LTexture();
		LTexture(std::string path); 
		//Deallocates memory
		~LTexture();
		//Loads image at specified pa
		//Renders texture at given point
		void render(SDL_Rect* clip=NULL  ,SDL_Rect* view=NULL,double angle=0.0,SDL_Point*center=NULL,SDL_RendererFlip flip=SDL_FLIP_NONE);
		//Gets image dimensions
		void setAlpha(Uint8 alpha=255);
		int getWidth();
		int getHeight();
		//The actual hardware texture
		SDL_Texture* mTexture;
		//Image dimensions
		bool loadFromFile( std::string path );
		//Deallocates texture
		void free();

};

class TTexture
{	 
		SDL_Texture* mTexture;
		TTF_Font* gFont;
		int mWidth;
		int mHeight;
	public:
		TTexture();
		bool setFont(string font_path,int size);
		bool loadFromRenderText(std::string texturetext,SDL_Color textcolor);
		void render(int x,int y);
		void free();	
		int getWidth() const ;
		int getHeight() const ;
};

/////////////////////////////////////////////////////////////////////////////////////
LTexture::LTexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
	SDL_SetTextureBlendMode(mTexture,SDL_BLENDMODE_BLEND);
}
LTexture::LTexture(std::string path)
{
	mTexture=NULL;
	mWidth=0;
	mHeight=0;
	if(!loadFromFile(path))
	{
		cout<<"Failed to load file! "<<path<<endl;
	}
	SDL_SetTextureBlendMode(mTexture,SDL_BLENDMODE_BLEND);
}
LTexture::~LTexture()
{
	//Deallocate
	free();
}              

bool LTexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}
void LTexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void LTexture::render(SDL_Rect* clip  ,SDL_Rect* view,double angle,SDL_Point* center,SDL_RendererFlip flip)
{
	if(clip==NULL)
	{
		clip->x=0;
		clip->y=0;
		clip->w=mWidth;
		clip->h=mHeight;
	}
	if(SDL_RenderCopyEx( gRenderer, mTexture,clip,view ,angle,center,flip)<0)
	{
		cout<<SDL_GetError()<<endl;
	}
	
}

int LTexture::getWidth()
{
	return mWidth;
}

int LTexture::getHeight()
{
	return mHeight;
}
void LTexture::setAlpha(Uint8 alpha)
{

	SDL_SetTextureAlphaMod(mTexture,alpha); 
}

///////////////////////////////////////////////////////////////////////////////////////
TTexture::TTexture()
{
	mTexture=NULL;
	mWidth=0;
	mHeight=0;
}
bool TTexture::loadFromRenderText(std::string texturetext,SDL_Color textcolor)
{
	
	SDL_Surface* textSurface=TTF_RenderText_Solid(gFont,texturetext.c_str(),textcolor);
	if(textSurface==NULL)
	{
		cout<<"Unable to create texture! "<<TTF_GetError()<<endl;
	}
	else
	{
		mTexture=SDL_CreateTextureFromSurface(gRenderer,textSurface);
		if(mTexture==NULL)
		{
			cout<<"Wrong! "<<SDL_GetError()<<endl;
		 }
		 else
		 {
			mWidth=textSurface->w;
			mHeight=textSurface->h;
			
		  } 
		SDL_FreeSurface(textSurface);
	}
	return mTexture!=NULL;
}

bool TTexture::setFont(string font_path,int size)
{
	gFont=TTF_OpenFont(font_path.c_str(),size);
	if(gFont==NULL)
	{
		cout<<"Fail to load font "<<font_path<<TTF_GetError()<<endl;
	}
	return gFont!=NULL;
}
void TTexture::render(int x,int y)
{
	SDL_Rect place={x,y,mWidth,mHeight};
	SDL_RenderCopy(gRenderer,mTexture,NULL,&place);
}
void TTexture::free()
{   
	if(mTexture!=NULL)
	{
		SDL_DestroyTexture(mTexture);
		mTexture=NULL;
		mHeight=0;
		mWidth=0;
		
	}
	TTF_CloseFont(gFont);
	gFont=NULL;
}
int TTexture::getHeight() const
{
	return mHeight;
}
int TTexture::getWidth() const
{
	return mWidth;
}
#endif
