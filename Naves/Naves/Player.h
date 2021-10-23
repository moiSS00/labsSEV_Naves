#pragma once

#include "Actor.h"
#include "Projectile.h" 
#include "Audio.h"

class Player : public Actor
{
public:
	Player(float x, float y, Game* game);
	void update();
	void moveX(float axis);
	void moveY(float axis);
	Projectile* shoot();
	
	void draw() override; // Va a sobrescribir
	void loseLife();

	int shootCadence = 30;
	int shootTime = 0;
	Audio* audioShoot;

	int lifes = 3;
	int invulnerableTime = 0;
};


