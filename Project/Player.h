#ifndef PLAYER_H
#define PLAYER_H
#include "class2.h"
#include "Sprite.h"
#include "Enemy.h"
#include "Item.h"
extern const int WALKING_ANIMATION_FRAMES;
extern const int ENEMY_NUM;
class Player:public Sprite
{
	int direction;
	int picture_index;
	int dx;
	int dy;
	const int vel_x;
	bool bumpy;
	bool landed;
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

};
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
	landed = true;
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

bool Player::move(gamebroad* game, Enemy* enemy)
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
	
	int counting = game -> blockcount;
	SDL_Rect tmpx = {Position.x+dx, Position.y, Position.w, Position.h};
	SDL_Rect tmpy = {Position.x, Position.y+dy, Position.w, Position.h};
	for(int i = 0; i < counting; i++)
	{
		if(game->bricks[0][i]->getType() == 3)
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
			{
				if(dx >= 0)
					Position.x = (game->bricks[0][i]->getBox().x) - Position.w;
				else
					Position.x = (game->bricks[0][i]->getBox().x) + game->bricks[0][i]->getBox().w;
				cout << "bump at (" << game->bricks[0][i]->mBox.x << ", " << game->bricks[0][i]->mBox.y << "): the bricks" << i <<endl;
			}
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
			{
				cout << "bump!" << endl;
				bumpy = true;
				landed = true;
				if(vel_y < 0 )
				{
					cout << "vel_y < 0"<<endl;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y + game->bricks[0][i]->mBox.h - Position.y +2;
	
				}
				else
				{
					cout << "vel_y >= 0"<<endl;
					in_air = false;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y - (Position.y + Position.h);
				}
			}
			
		}	
		else if(game->bricks[0][i]->getType() == 2)
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
				gameover = true;
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
				gameover = true;
		}
		else
		{
			if(checkCollision(tmpx, game->bricks[0][i]->mBox))
			{
				dx = 0;
				cout << "bump at (" << game->bricks[0][i]->mBox.x << ", " << game->bricks[0][i]->mBox.y << "): the bricks" << i <<endl;
			}
			
			if(checkCollision(tmpy, game->bricks[0][i]->mBox))
			{
				cout << "bump!" << endl;
				bumpy = true;
				landed = true;
				if(vel_y < 0 )
				{
					cout << "vel_y < 0"<<endl;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y + game->bricks[0][i]->mBox.h - Position.y +2;
	
				}
				else
				{
					cout << "vel_y >= 0"<<endl;
					in_air = false;
					vel_y = 0;
					dy = game->bricks[0][i]->mBox.y - (Position.y + Position.h);
				}
			}
		}
		
	}
	
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
		cout << "change" << endl;
	}
	
	cout << "bumped = " << bumpy << ",   in_air = " << in_air << ",   jumped = " << jumped << endl;
	cout << "Position: " << Position.x << ", " << Position.y << endl;
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
#endif
