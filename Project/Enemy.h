#ifndef ENEMY_H
#define ENEMY_H
#include "Sprite.h"
class Enemy: public Sprite
{
	int direction;
	int picture_index;
	int counter;
	const int MOVING_VEL;
public:
	//Initializes variables
	Enemy();
	~Enemy();
	void setPosition(int x, int y);
	void update();
	int getIndex();
	//Deallocates memory

	//Loads image at specified pa
	//Renders texture at given point
};
extern const int WALKING_ANIMATION_FRAMES;

Enemy::Enemy():MOVING_VEL(1)
{
	direction = 1;
	picture_index = 0;
	counter = 0;
	Position.w = 30;
	Position.h = 57;
}

Enemy::~Enemy()
{
	spriteTexture.free();
}

void Enemy::setPosition(int x, int y)
{
	
	Position.x = x;
	Position.y = y;
}

int Enemy::getIndex()
{
	if(direction == 1)
		return picture_index;
	else if(direction == -1)
		return picture_index + 64;
		
}

void Enemy::update()
{
	counter += MOVING_VEL * direction; 
	if(counter >= 64)
		direction = -1;
	else if(counter <= -64)
		direction = 1;
	Position.x += MOVING_VEL * direction;
	picture_index++;
	
	if(picture_index / 8 >= WALKING_ANIMATION_FRAMES)
		picture_index = 0;
}
#endif
