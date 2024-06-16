#ifndef CLASS3_H
#define CLASS3_H
#include "class5.h"
#include "Sprite.h"
#include "Button.h"
#include "Item.h"
#include "Enemy.h"
enum blocktype
{
	BLOCK_TYPE,
	FALLINGBLOCK_TYPE,
	DANCINGBLOCK_TYPE,
	CYCLEBLOCK_TYPE
};
const int BLOCKTYPENUM=4;
const int ITEM_NUM = 5;
extern const int WALKING_ANIMATION_FRAMES;
extern const int ENEMY_NUM;

class gamebroad
{
	friend class Player;
	private:
		block* bricks[BLOCKTYPENUM][BLOCK_N];
		fort* shooter;		
		Item* book[ITEM_NUM];
		Timer timerecord;
		//int blockcount[BLOCKTYPENUM];
		int blockcount;
		int fallingblockcount;
		int dancingblockcount;	
		TTexture levelnumber;
		TTexture timecounter;
		int Itemcount;
		int Enemycount;
		Button Pause;
		void smallviewport(const SDL_Rect& camera);
	public :
		gamebroad(int level);  
		~gamebroad();
		void render(SDL_Rect& camera,bool& pause);
		void movement();
		void handleEvent(SDL_Event& e,SDL_Rect& camera,bool& pause);

};
class Player:public Sprite
{
	int direction;
	int picture_index;
	int dx;
	int dy;
	const int vel_x;
	bool bumpy;
	static int vel_y;
	bool gameover;
	
public:
	//Initializes variables
	Player();
	~Player();
	//Deallocates memory

	//Loads image at specified pa
	//Renders texture at given point
	//void render(SDL_Rect&camera,SDL_Rect* clip=NULL);
	int getIndex();
	bool in_air;
	bool jumped;
	void update(SDL_Event& e, gamebroad* game);
	void handle_input(const Uint8* keystate, gamebroad* game);
	bool move(gamebroad*, Enemy*);
	void reset();
	bool handleCollision(SDL_Rect, SDL_Rect, gamebroad*);

};

class transition :public LTexture
{
		int alpha;
		bool coverd;
		int vel;
	public:
		transition(string path);
		~transition(){};
		void change(bool& check,bool& cover);
};
class cycleblock: public block
{
	public:
		cycleblock(int x,int y,int type);
		~cycleblock(){};
		void blockmove();
};
gamebroad::gamebroad(int level):Pause("./Pause.png")
{
	shooter=NULL;
	Pause.setposition(SCREEN_WIDTH/2-40,20,80,80);
	int x=0,y=0;
	for(int i=0;i<BLOCKTYPENUM;i++)
	{
		for(int j=0;j<BLOCK_N;j++)
			bricks[i][j]=NULL;
	//	blockcount[i]=0;
	}
	blockcount=0;
	fallingblockcount=0;
	dancingblockcount=0;	
	Itemcount = 0;
	Enemycount = 0;
	for(int i = 0; i < ITEM_NUM; i++)
		book[i] = NULL;
		
	stringstream leveltext;
	leveltext.str("");
	leveltext<<"./level"<<level<<".txt";
	std::ifstream map(leveltext.str().c_str());
	levelnumber.setFont("./orange juice 2.0.ttf",40);
	leveltext.str("");
	leveltext<<"Level "<<level;
	SDL_Color white={255,255,255,255};
	levelnumber.loadFromRenderText(leveltext.str().c_str(),white);
//	timerecord.start();
	if(map.fail())
	{
		cout<<"Unable to load map file!\n"<<endl;
	}
	else
	{
		
		for(int i=0;i<BLOCK_N;i++)
		{
			int type;
			map>>type;
			if(map.fail())
			{
				cout<<"Unexpected end of file!\n";
				break;
			}
			if((type>0)&&(type<5))
			{
				bricks[0][blockcount]=new block(x,y,type);
				blockcount++;
			}
			if(type==5)
			{
				bricks[FALLINGBLOCK_TYPE][fallingblockcount]=new fallingblock(x,y,type,fallingblockcount);
				fallingblockcount++;
			}
			if(type==6)
			{
				bricks[DANCINGBLOCK_TYPE][dancingblockcount]=new dancingblock(x,y,type);
				dancingblockcount++;
			}
			if(type==7)
			{
				/*
				bricks[CYCLEBLOCK_TYPE][blockcount[CYCLEBLOCK_TYPE]]=new cycleblock(x,y,type);
				blockcount[CYCLEBLOCK_TYPE]++;*/
				if(type == 7)
				{
					book[Itemcount] = new Item(x,y);
					book[Itemcount]->loadFromFile("./book.png");
					Itemcount++;
				}
			}
			
			if(type==25)
			{
				shooter=new fort(x,y);
			}
			x+=TILE_WIDTH;
			if(x>=MAP_WIDTH)
			{
				x=0;
				y+=TILE_HEIGHT;
			}
		}
			
	}
//	cout<<"Itemcount: "<<Itemcount<<endl;
//	cout<<888<<endl;
	map.close();
	
}
gamebroad::~gamebroad()
{
	timerecord.stop();
}
void gamebroad::render(SDL_Rect& camera,bool& pause)
{
	if(!pause)
	{
		movement();
	//	cout<<888<<endl;
		if(timerecord.isPaused())
			timerecord.unpause();
	}else
	{
		timerecord.pause();
	}
	//cout<<1<<endl;
	if(timerecord.getTicks()==0)
		timerecord.start();
	for(int i = 0; book[i]!= NULL; i++)
	{
		book[i]->render(camera);
		//cout<<2<<endl;
	}
//	cout<<8<<endl;
	for(int i=0;i<BLOCKTYPENUM;i++)
	{
		for(int j=0;bricks[i][j]!=NULL;j++)
		{
			bricks[i][j]->render(camera);
		}
	}
	//cout<<9<<endl;
	if(shooter!=NULL)
		shooter->render(camera);
	stringstream timetext;
	timetext.str("");
	timetext<<(timerecord.getTicks()/(60*60*1000))<<" : "<<(timerecord.getTicks()/(60*1000))%60<<" : "<<(timerecord.getTicks()/1000)%60;
	smallviewport(camera);	
	levelnumber.render(20,20);
	SDL_Color white={255,255,255,255};
	timecounter.setFont("./orange juice 2.0.ttf",40);
	timecounter.loadFromRenderText(timetext.str().c_str(),white);
	timecounter.render(SCREEN_WIDTH-timecounter.getWidth()-20,20);
//	cout<<10<<endl;
	Pause.render();
//	cout<<11<<endl;
}
void gamebroad::movement()
{
	for(int i=0;i<BLOCKTYPENUM;i++)
	{
		for(int j=0;bricks[i][j]!=NULL;j++)
		{
			bricks[i][j]->blockmove();
		}
	}
}
void gamebroad::smallviewport(const SDL_Rect& camera)
{
		SDL_SetRenderDrawColor(gRenderer,192,192,192,255);
		int y=60;
		SDL_Rect fillrect={10,y,160,120};
		SDL_RenderFillRect(gRenderer,&fillrect); 
		for(int i=0;i<BLOCKTYPENUM;i++)
		{
			for(int j=0;bricks[i][j]!=NULL;j++)
			{
				if(bricks[i][j]->getBox().x<=MAP_WIDTH-TILE_WIDTH&&bricks[i][j]->getBox().x>=0&&bricks[i][j]->getBox().y<=MAP_HEIGHT-TILE_HEIGHT&&bricks[i][j]->getBox().y>=0)
				{
					SDL_SetRenderDrawColor(gRenderer,128,138,135,255);
					SDL_Rect fillrect2={10+(bricks[i][j]->getBox().x)/8,y+(bricks[i][j]->getBox().y)/8,8,8};
					SDL_RenderFillRect(gRenderer,&fillrect2);
				}
			
			}
		}
		SDL_SetRenderDrawColor(gRenderer,255,0,0,255);
		SDL_Rect camerarect={10+camera.x/8,y+camera.y/8,camera.w/8,camera.h/8};
		SDL_RenderDrawRect(gRenderer,&camerarect);
		
}
void gamebroad::handleEvent(SDL_Event& e,SDL_Rect& camera,bool& pause)
{
	if(shooter!=NULL)
	{
		shooter->handleEvent( e, camera);
	}
	if(!pause)
	{
		Pause.handleEvent(e);
		if(Pause.getpressed())
			pause=true;
	}
		
	
}
//////////////////////////////////////////////////////////////////////////////////
Player::Player():vel_x(5)
{
	reset();
}

void Player::reset()
{
	//Initialize

	direction = 1;
	picture_index = 0;
	Position.x = 70;
	Position.y = 841;
	Position.w = 31;
	Position.h = 55;
	jumped = false;
	in_air = false;
	dx = 0;
	dy = 0;
	//vel_y = 0;
	bumpy = false;
	gameover = false;
}

int Player::vel_y = 0;
Player::~Player()
{
	spriteTexture.free();
}

int Player::getIndex()
{
	if(direction == 1)
		return picture_index;
	else
		return picture_index + 16;
}


void Player::handle_input(const Uint8* keystates, gamebroad* game)
{
	cout << endl;
	dx = 0;
	dy = 0;

	if(keystates[SDL_SCANCODE_LEFT])
	{
		dx = -3;
	}
	if(keystates[SDL_SCANCODE_RIGHT])
	{
		dx = 3;
    }
    
	if(keystates[SDL_SCANCODE_SPACE])
	{
		if(!jumped && !in_air)
		{
			vel_y -= 15;
			jumped = true;
			in_air = true;
		}
	}
	if(!keystates[SDL_SCANCODE_SPACE] && !in_air)
		jumped = false;

	
}

bool Player::move(gamebroad* game, Enemy*enemy)
{
	// add gravity
	static int counter = 0;
	dy = vel_y;
	//if(in_air)
	counter++;
	if(counter % 2 == 0)
		vel_y++;
		
	if(Position.x + dx < 0 || (Position.x + dx + Position.w > MAP_WIDTH))
		dx = 0;
	if(Position.y + dy < 0 || (Position.y + dy + Position.h > MAP_HEIGHT))
		dy = 0;
	if(Position.y < 64)
		Position.y = 64; 
	
	
	
	SDL_Rect tmpx = {Position.x+dx, Position.y, Position.w, Position.h};
	SDL_Rect tmpy = {Position.x, Position.y+dy, Position.w, Position.h};
	
	gameover = handleCollision(tmpx, tmpy, game);
	
	
	for(int i = 0; i < game-> Itemcount; i++)
	{
		if(checkCollision(tmpx, game->book[i]->getPos()))
			game->book[i]->gotten(true);
		if(checkCollision(tmpy, game->book[i]->getPos()))
			game->book[i]->gotten(true);
	}
	
	
	for(int i = 0; i < ENEMY_NUM; i++)
	{
		if(checkCollision(tmpx, *(enemy[i].getPos())))
			gameover = true;
		if(checkCollision(tmpy, *(enemy[i].getPos())))
			gameover = true;
	}
	
	
	if(gameover)
	{
		Position.y += 20;
		return gameover;
	}
	
	if(!bumpy)
	{
		in_air = true;
	}
	
	//cout << "bumped = " << bumpy << ",   in_air = " << in_air << ",   jumped = " << jumped << endl;
	//cout << "Position: " << Position.x << ", " << Position.y << endl;
	Position.x += dx;
	Position.y += dy;
	if(dx > 0)
	{
		picture_index++;
		direction = 1;
	}
	else if(dx < 0)
	{
		picture_index++;
		direction = -1;
	}
	if( picture_index / 2 >= WALKING_ANIMATION_FRAMES )
	{
		picture_index = 0;
	}
	return gameover;
}
bool Player::handleCollision(SDL_Rect tmpx, SDL_Rect tmpy, gamebroad* game)
{
	for(int i = 0; i < game->blockcount; i++)
	{
		if(game->bricks[0][i]->getType() == 2)  // Collision with flunks
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
				return true;
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
				return true;
			
		}	
		else if(game->bricks[0][i]->getType() == 3) // Collision with moving blocks
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
			{
				if(dx >= 0)
					Position.x = (game->bricks[0][i]->getBox().x) - Position.w;
				else
					Position.x = (game->bricks[0][i]->getBox().x) + game->bricks[0][i]->getBox().w;
				//cout << "bump at (" << game->bricks[0][i]->mBox.x << ", " << game->bricks[0][i]->mBox.y << "): the bricks" << i <<endl;
			}
			else if (checkCollision(tmpy, game->bricks[0][i]->mBox))
			{
				//cout << "bump!" << endl;
				bumpy = true;
				if(vel_y < 0 )
				{
					//cout << "vel_y < 0"<<endl;
					vel_y = -1;
					dy = game->bricks[0][i]->mBox.y + game->bricks[0][i]->mBox.h - Position.y +2;
	
				}
				else
				{
					//cout << "vel_y >= 0"<<endl;
					in_air = false;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y - (Position.y + Position.h);
				}
				
			}
		}
		else if(game->bricks[0][i]->getType() == 4)  // Collision with spiked blocks
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
			{
				dx = 0;
			}
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
			{
				if(vel_y <= 0 )
				{
					vel_y = -1;
					dy = game->bricks[0][i]->mBox.y + game->bricks[0][i]->mBox.h - Position.y +2;
				}
				else
				{
					return true;
				}
			}
		}
		else  // Collision with other normal blocks
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
				dx = 0;
				//cout << "bump at (" << game->bricks[0][i]->mBox.x << ", " << game->bricks[0][i]->mBox.y << "): the bricks" << i <<endl;
			
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
			{
				//cout << "bump!" << endl;
				bumpy = true;
				if(vel_y < 0 )
				{
					//cout << "vel_y < 0"<<endl;
					vel_y = -1;
					dy = game->bricks[0][i]->mBox.y + game->bricks[0][i]->mBox.h - Position.y +2;
	
				}
				else
				{
					//cout << "vel_y >= 0"<<endl;
					in_air = false;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y - (Position.y + Position.h);
				}
			}
		}
		
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
transition::transition(string path):LTexture(path),alpha(1),coverd(false),vel(5)
{
	setAlpha(alpha);
}
void transition::change(bool& check,bool& cover)
{
	
	if(alpha>240)
	{
		vel=-vel;
		cover=true;
	}
	alpha+=vel;
	if(alpha<0)    
	{
		check=true;
		vel=-vel;  
		cover=false;        
		return;
	}

	setAlpha(alpha);
	return;

}
cycleblock::cycleblock(int x,int y,int type):block(x,y,type)
{
	velx=5;
}
void cycleblock::blockmove()
{
	mBox.x+=velx;
	if(mBox.x>=MAP_WIDTH)
		mBox.x=0; 
}
#endif
