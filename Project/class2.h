#ifndef CLASS2_H
#define CLASS2_H
#include "class.h"
#include "Timer.h"
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>

const int TILE_WIDTH=64;
const int TILE_HEIGHT=64;
const int MAP_WIDTH=1280;
const int MAP_HEIGHT=960;
const int BLOCK_N=300;
bool checkCollision(SDL_Rect,SDL_Rect);


class block
{
	static LTexture gTileTexture;	
	static SDL_Rect gTileClips[12];
	public:
		block(int x,int y,int blocktype);
		~block();
		virtual void render(SDL_Rect& camera);
		int getType()const;
		SDL_Rect getBox()const;
		virtual void blockmove();
		SDL_Rect mBox;	
		int orix,oriy;
		int velx;
		int mType;
	
};

class fallingblock :public block
{
	static Timer clock;
	static int count;
	public:
		fallingblock(int x,int y,int type,int i);
		~fallingblock();
		void blockmove();
		int getindex(){return index	;}
		bool getstart(){return start;	}
		bool getfall(){return fall	;}
	private:
		int fallv;
		int upv;
		int index;
		bool start;
		bool fall;
};  
class dancingblock:public block
{
	public:
		dancingblock(int x,int y,int type);
		~dancingblock(){};
		void blockmove();
	private:
		float vely;	
};

LTexture block::gTileTexture;
SDL_Rect block::gTileClips[12]={{0,0,80,80},{0,80,80,80},{0,2*80,80,80},{0,80,80,80},{80,0,80,80},{80,80,80,80},{0,80,80,80},{0,80,80,80}};
Timer fallingblock::clock;
int fallingblock::count=0;
block::block(int x,int y,int tiletype)
{
	mBox.x=x;
	mBox.y=y;
	orix=x;
	oriy=y;  
	if(tiletype==3)
	{
		velx=1;
	}else
		velx=0;
	mBox.w=TILE_WIDTH;
	mBox.h=TILE_HEIGHT;
	mType=tiletype;
	if(gTileTexture.mTexture==NULL)
	{
		if(!gTileTexture.loadFromFile("./all_block.png"))
		{
			cout<<"Failed to load from file: ./different blocks.png"<<endl;
		}
	}
}   
block::~block()
{
	gTileTexture.free();
}
void block::render(SDL_Rect& camera)
{
	
	if(checkCollision(camera,mBox))
	{
		SDL_Rect view={mBox.x-camera.x,mBox.y-camera.y,TILE_WIDTH,TILE_HEIGHT};
		gTileTexture.render(&gTileClips[mType],&view);
	}
}
int block::getType()const
{
	return mType;
}
SDL_Rect block::getBox()const
{  
	return mBox;
}
void block::blockmove()
{
	if(mType==3)
	{
		if(mBox.x<orix||mBox.x>orix+2*TILE_HEIGHT)
		{
			velx=-velx;
		} 
		mBox.x+=velx;
		
	}
	return ;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
fallingblock::fallingblock(int x,int y,int type,int i):block(x,y,type)
{
	fallv=5;
	upv=2;
	index=i;
	start=false;
	fall=false;
	count++;
}
fallingblock::~fallingblock()
{
	;
}
void fallingblock::blockmove()
{
	if(index==0&&!start&&!fall)
	{
		start=true;
		clock.start();
	}
	if(index>0&&!start&&!fall)
	{
		if(clock.getTicks()>index*300)
		{
			start=true;
		}
	}
	if(!fall&&start)
	{
		mBox.y+=fallv;
		if(mBox.y>oriy+TILE_WIDTH)
		{
			mBox.y=oriy+TILE_WIDTH;
			fall=true;
		}
	}
	if(fall&&start&&index==count-1)
	{
		clock.stop();
	}
	if(fall&&start&&clock.getTicks()==0)
	{
		start=false;
	}
	if(!start&&fall)
	{
		mBox.y-=upv;
		if(mBox.y<oriy)
		{
			mBox.y=oriy;
			fall=false;
		}
	}
} 
//////////////////////////////////////////////////////////////////////////////
dancingblock::dancingblock(int x,int y,int type) :block(x,y,type)
{
	//srand(time(0));
	vely=(rand()%3+2);
	if(rand()%2)
		vely=-vely; 
//	cout<<vely<<" "<<mBox.y<<endl;
}
void dancingblock::blockmove()
{
	if(mBox.y<oriy-mBox.h||mBox.y>oriy+mBox.h)
	{
		vely=-vely;
	 }
	mBox.y+=vely; 
}
//////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
bool checkCollision( SDL_Rect a, SDL_Rect b )
{
    //The sides of the rectangles   
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    //Calculate the sides of rect A
    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    //Calculate the sides of rect B
    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB||topA >= bottomB||rightA <= leftB||leftA >= rightB  )
    {
        return false;
    }
    //If none of the sides from A are outside B
    return true;
}

////////////////////////////////////////////////////////////////////////////////////////

#endif
