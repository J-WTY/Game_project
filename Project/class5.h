#ifndef CLASS5_H
#define CLASS5_H
#include "class4.h"  
#include "Timer.h"
const int MISSILE_WIDTH=18;
const int MISSILE_HEIGHT=51;
class explosion
{
		LTexture explosionsheet;
		static SDL_Rect explosionclips[4];
		int frame;
		SDL_Rect mBox;
	public:
		explosion(int x,int y);
		void render(SDL_Rect& camera,bool&);
};
class linear_missile
{
		LTexture Lmissile;
		explosion* end; 
		vector<double> velocity;
		SDL_Rect mBox;
		SDL_Rect mcamera;
		Timer liferecord;
		double mangle;
		bool exit;
	public:
		linear_missile(int x,int y,double angle);
		~linear_missile();
		void missilemove();
		void render(const SDL_Rect& camera);
		void vanish();
}; 
class fort
{
		SDL_Rect mBox;
		SDL_Point center;
		double angle;
		vector<int> pos;
		Timer launchrecord;
		linear_missile* Lmissile;
		LTexture fortTexture;
	public:
		fort(int x,int y);
		~fort(){};
		void rotate( vector<int>& target,SDL_Rect& camera);
		void render(const SDL_Rect& camera);
		void handleEvent(SDL_Event& e,SDL_Rect& camera);
		void launch();
};

SDL_Rect explosion::explosionclips[4]={{0,0,614,620},{614,0,614,620},{1228,0,614,620},{1842,0,614,620}};
explosion::explosion(int x,int y):explosionsheet("./explosion.png")
{
	mBox.x=x;
	mBox.y=y;
	mBox.w=TILE_WIDTH;
	mBox.h=TILE_HEIGHT;
	frame=0;
}
void explosion::render(SDL_Rect& camera,bool& finish)
{
	frame++;
	if(frame>=16)
		finish=false;
//	cout<<frame<<endl;
	if(checkCollision(camera,mBox))
	{
		SDL_Rect view={mBox.x-camera.x,mBox.y-camera.y,TILE_WIDTH,TILE_HEIGHT};
		explosionsheet.render(&explosionclips[frame/4],&view);
	}
}
fort::fort(int x,int y):fortTexture("./fort.png"),pos(x+TILE_WIDTH/2,y+TILE_HEIGHT/2)
{
	mBox.x=x;
	mBox.y=y;
	mBox.w=TILE_WIDTH;
	mBox.h=TILE_HEIGHT;
	center.x=x+TILE_WIDTH/2;
	center.y=y+TILE_HEIGHT/2;
	angle=0.0;
	Lmissile=NULL;
	launchrecord.start();
	Lmissile=new linear_missile(mBox.x,mBox.y,angle);
}  
void fort::rotate( vector<int>& target,SDL_Rect& camera)
{
	pos.x=mBox.x-camera.x+TILE_WIDTH/2;
	pos.y=mBox.y-camera.y+TILE_HEIGHT/2;
	angle= (target-pos).angle();
	return;
}
void fort::render(const SDL_Rect& camera)
{
	launch();
	if(Lmissile!=NULL)
	{
	//	cout<<"oooo"<<endl;
			Lmissile->missilemove();
		Lmissile->render(camera);
	
	}

	if(checkCollision(camera,mBox))
	{
		center.x=mBox.x-camera.x;
		center.y=mBox.y-camera.y;
		SDL_Rect view={mBox.x-camera.x,mBox.y-camera.y,TILE_WIDTH,TILE_HEIGHT};
		SDL_Rect clip={0,0,fortTexture.getWidth(),fortTexture.getHeight()};
		fortTexture.render(&clip,&view,angle,NULL,SDL_FLIP_NONE);
	}
}
void fort::handleEvent(SDL_Event& e,SDL_Rect& camera)
{
	if(e.type==SDL_MOUSEMOTION)
	{
		int x,y;
		SDL_GetMouseState(&x,&y);
		vector<int> target(x,y);
		rotate(target,camera);
	}
	return;
}
void fort::launch()
{
	if(launchrecord.getTicks()>=3000)
	{
		delete Lmissile;
		launchrecord.stop();
		launchrecord.start();
		Lmissile=new linear_missile(mBox.x+TILE_WIDTH/2,mBox.y,angle);
	}
}
linear_missile::linear_missile(int x,int y,double angle):Lmissile("./linear_missile.png"),velocity(0,-4)
{
	mBox.x=x;
	mBox.y=y;
	mBox.w=MISSILE_WIDTH;
	mBox.h=MISSILE_HEIGHT;
	velocity.rotate(angle);
	end=NULL;
	//cout<<velocity.angle()-angle<<endl;
/*	if(velocity.angle()-angle>30||velocity.angle()-angle<-30)
	{
		cout<<"angle: "<<angle<<endl;
		cout<<velocity.x<<" "<<velocity.y<<endl;
	}*/
		
	mangle=angle;
	exit=true;
	liferecord.start();
}
void linear_missile::missilemove()
{
	if(exit)
	{
		mBox.x+=int(velocity.x);
		mBox.y+=int(velocity.y);
	//	cout<<"x: "<<mBox.x<<"y: "<<mBox.y<<endl;
		if(mBox.x<0||mBox.x+MISSILE_HEIGHT>MAP_WIDTH||mBox.y<0||mBox.y+MISSILE_HEIGHT>MAP_HEIGHT)
		{
			vanish();
		}
		if(liferecord.getTicks()>=2500)
		{	
			vanish();
		}
	}
	
}
void linear_missile::render(const SDL_Rect& camera)
{
	mcamera=camera;
	if(checkCollision(camera,mBox)&&exit)
	{
		SDL_Rect view={mBox.x-camera.x,mBox.y-camera.y,MISSILE_WIDTH,MISSILE_HEIGHT};
		SDL_Rect clip={0,0,Lmissile.getWidth(),Lmissile.getHeight()};
		Lmissile.render(&clip,&view,mangle,NULL,SDL_FLIP_NONE);
	}
}
linear_missile::~linear_missile()
{
	if(exit==true)
		vanish();
}
void linear_missile::vanish()
{
	if(end==NULL)
		end=new explosion(mBox.x,mBox.y);

	end->render(mcamera,exit);
	if(exit==false)
	{
		liferecord.stop();
	}
		
	
}
#endif
