#include "class.h"
#include "class2.h"
#include "class3.h"
#include "class4.h"
#include "class5.h"
#include "Normalsurface.h"
#include "Button.h"
#include "Timer.h"
#include "chooselevelsurface.h"
#include "PauseSurface.h"
#include "Item.h"
#include "Sprite.h"
#include "Enemy.h"
//Starts up SDL and creates window
bool init();   
                                
//Loads media    
bool loadMedia();
    
//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
//extern SDL_Renderer* gRenderer;

LTexture gTexture;
Player gPlayer;
const int ENEMY_NUM = 3;
const int WALKING_ANIMATION_FRAMES = 8;

Enemy gEnemy[ENEMY_NUM];
SDL_Rect gPlayerClips[WALKING_ANIMATION_FRAMES];
SDL_Rect gEnemyClips[WALKING_ANIMATION_FRAMES*2];
bool init()
{  
	//Initialization flag
	bool success = true;
    
	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else    
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{    
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_mage Error: %s\n", IMG_GetError() );
					success = false;
				}
				if(TTF_Init()==-1)
				{
					cout<<"ttf could not be initialiazed."<<TTF_GetError()<<endl;
					success=false;
				}
				if(!gPlayer.loadFromFile("./guy.png"))
				{
					printf("gPlayer could not be initialized! SDL_image Error: %s\n", IMG_GetError());
					success = false;
				}
				for(int i = 0; i < ENEMY_NUM; i++)
				{
					if(!gEnemy[i].loadFromFile("./Teacher.png"))
					{
						printf("gEnemy coculd not be initialized! SDL_image Error: %s\n", IMG_GetError());
						success = false;
					}
				}
				gEnemy[0].setPosition(755, 640);
				gEnemy[1].setPosition(330, 192);
				gEnemy[2].setPosition(810, 833);
				
				
				for(int i = 0; i < WALKING_ANIMATION_FRAMES; i++)
				{
					gPlayerClips[i].w = 31;
					gPlayerClips[i].h = 55;
					gPlayerClips[i].y = 0;
					gEnemyClips[i].w = 53;
					gEnemyClips[i].h = 57;
					gEnemyClips[i].y = 0;
					gEnemyClips[i+WALKING_ANIMATION_FRAMES].w = 53;
					gEnemyClips[i+WALKING_ANIMATION_FRAMES].h = 60;
					gEnemyClips[i+WALKING_ANIMATION_FRAMES].y = 62;
				}
				
				gPlayerClips[ 0 ].x =   0;
				gPlayerClips[ 1 ].x =  31;
				gPlayerClips[ 2 ].x =  62;
				gPlayerClips[ 3 ].x =  95;
				gPlayerClips[ 4 ].x = 126;
				gPlayerClips[ 5 ].x = 159;
				gPlayerClips[ 6 ].x = 192;
				gPlayerClips[ 7 ].x = 224;

				for(int i = 0; i < 2; i++)
				{
					gEnemyClips[ 0 + i*WALKING_ANIMATION_FRAMES].x =  15;
					gEnemyClips[ 1 + i*WALKING_ANIMATION_FRAMES].x =  87;
					gEnemyClips[ 2 + i*WALKING_ANIMATION_FRAMES].x = 158;
					gEnemyClips[ 3 + i*WALKING_ANIMATION_FRAMES].x = 229;
					gEnemyClips[ 4 + i*WALKING_ANIMATION_FRAMES].x = 301;
					gEnemyClips[ 5 + i*WALKING_ANIMATION_FRAMES].x = 374;
					gEnemyClips[ 6 + i*WALKING_ANIMATION_FRAMES].x = 446;
					gEnemyClips[ 7 + i*WALKING_ANIMATION_FRAMES].x = 514;
				}
			}  
		}   
	}

	return success;
}         
void close() 
{
	//Free loaded image
	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
	TTF_Quit();
}
const int LEVELNUM=3;
int main( int argc, char* args[] )
{
	//Start up SDL and create window    
	if( !init() )  
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{      
		//Load me dia       
			SDL_Color black={255,255,255};
			Normalsurface StartBackground("./full of books.png" ,"press blank to continue...","./orange juice 2.0.ttf",black,40);
			chooselevelsurface clf;
			gamebroad* game[LEVELNUM];  
			PauseSurface pSurface;
			for(int i=0;i<LEVELNUM;i++)
			{
				game[i]=new gamebroad(i+1);
		   }       
				
			transition transurface("./transition.png");  
		 //	block.loadFromFile("./different blocks.png");
			//Main loop flag
			SDL_Rect camera={0,MAP_HEIGHT-SCREEN_HEIGHT,SCREEN_WIDTH,SCREEN_HEIGHT};
			bool quit = false;
			int level=0;
			int index=0;
			bool cover=false;
			bool transform=true;
			bool transtart=false;
			bool again=false;
			bool pause=false;
			bool gameover=false;
			SDL_Event e;
			//While application is running
			while( !quit )     
			{     
				//Handle events on queu    
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit 
					const Uint8* keystate = SDL_GetKeyboardState(NULL);
					if( e.type == SDL_QUIT )
					{
						quit = true;
					} 
					if(level==0&&index==0)
					{
					//	cout<<"jjj"<<endl;    
						StartBackground.handleevent(level,e);      
					}
					if(level==0 && keystate[SDL_SCANCODE_RETURN])
					{
						/*
						StartBackground.handleevent(level);
						camera.x = gPlayer.getPos()->x -70;
						camera.y = gPlayer.getPos()->y-361;*/
					}
					else if(level > 0&&!pause)
					{
						gPlayer.handle_input(keystate, game[level-1]);
						
					} 
					if(level==-1&&index==-1)
					{
						clf.handleEvent(e,level);
					}
					if(e.type==SDL_KEYDOWN)
					{
						if(level>0&&!pause)
						{
							/*
							switch(e.key.keysym.sym)
							{
								case SDLK_UP:
									camera.y-=10;
									break;
								case SDLK_DOWN:
									camera.y+=10;
									break;
								case SDLK_LEFT:
									camera.x-=10;
						 			break;
								case SDLK_RIGHT:
									camera.x+=10;
									break;
							}            
							if(camera.x<0)
								camera.x=0;
							if(camera.y<0)   
							{     
								camera.y=0;
							}     
							if(camera.x+SCREEN_WIDTH>MAP_WIDTH)
							{
								camera.x=MAP_WIDTH-SCREEN_WIDTH;
							}
							if(camera.y+SCREEN_HEIGHT>MAP_HEIGHT)
							{
								camera.y=MAP_HEIGHT-SCREEN_HEIGHT;
							 } */
						}  
						
					}
					if(level>0&&!pause)
					{
						if(game[level-1]!=NULL)
						{
							game[level-1]->handleEvent(e,camera,pause);
						}  
					}
					if(pause)
					{
					//	cout<<"jjjj"<<endl;  
						pSurface.handleEvent(e,level,pause);
					}
					   
				}      
				                    
				SDL_SetRenderDrawColor( gRenderer, 0, 0, 0, 0xFF );
				SDL_RenderClear( gRenderer );
			 
				SDL_Rect* playerClip = &gPlayerClips[ gPlayer.getIndex() / 4 ];
				SDL_Rect* enemyClip = &gEnemyClips[ gEnemy[0].getIndex() / 8 ];
				
				if(level!=index||again)
				{	
					//cout<<0<<endl;
				//	cout<<"level: "<<level<<" index: "<<index<<endl;
					if(!transtart)
					{  
						//cout<<1<<endl;
						transurface.change(transtart,cover);
						if(cover&&transform)
						{   
	    					
						}else if(!transtart&&!cover)
						{   
						//	cout<<2<<endl;
							if(index==0)
								StartBackground.render(0,0,0,0);
							else if(index>0)
								game[index]->render(camera,pause);
							else if(index==-1)
								clf.render();
								
						}
						          
						SDL_Rect clip={0,0,SCREEN_WIDTH,SCREEN_HEIGHT};
						transurface.render(&clip,&clip);
					}	   
					else
					{
						index=level;
						transtart=false;     
						cover=false;
						transform=true;
						again=false;
					}
				                      
				 }
				  
				if(level==0&&index==0)
				{
					StartBackground.render(0,0,0,0);
				}               
				 if(index>0&&index==level)         
				 {       
				 	if(game[level-1]==NULL)
						game[level-1]=new gamebroad(level);
				 	game[level-1]->render(camera,pause);
				 	camera.x = gPlayer.getPos()->x -SCREEN_WIDTH/2;
					camera.y = gPlayer.getPos()->y-SCREEN_HEIGHT/2;
					if(gPlayer.getPos()->x>MAP_WIDTH-TILE_WIDTH&&gPlayer.getPos()->y<2*TILE_HEIGHT)
					{
						level++;
						gPlayer.reset();
					}
					if(camera.x<0)
						camera.x=0;
					if(camera.y<0)
					{
						camera.y=0;
					}
					if(camera.x+SCREEN_WIDTH>MAP_WIDTH)
					{
						camera.x=MAP_WIDTH-SCREEN_WIDTH;
					}
					if(camera.y+SCREEN_HEIGHT>MAP_HEIGHT)
					{
						camera.y=MAP_HEIGHT-SCREEN_HEIGHT;
					}
					
					if(gPlayer.move(game[level-1], gEnemy))
						gPlayer.reset();	
					for(int i = 0; i < ENEMY_NUM; i++)
						gEnemy[i].update();
					
					gPlayer.render(camera, playerClip);
					for(int i = 0; i < ENEMY_NUM; i++)
						gEnemy[i].render(camera, enemyClip);
						
				}     
				if(level==-1&&index==-1)
				{
						clf.render(); 
				}      
				if(pause)
				{
					pSurface.render(pause);
				}
				SDL_RenderPresent( gRenderer );
			//	game.setviewport(camera);
			}           
		    SDL_Delay(100); 
	} 

	//Free resources and close SDL
	close();

	return 0;
}
