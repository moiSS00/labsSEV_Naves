#pragma once

#include "Actor.h"
#include "Animation.h" 

class Enemy : public Actor
{
public:
	Enemy(float x, float y, Game* game);
	void update();
	void draw() override; // Va a sobrescribir
	void loseLife(); // Metodo que hace perder una vida al enemigo

	Animation* aMoving;
	Animation* animation; // Referencia a la animación mostrada

	int lifes = 3; // Número de vidas
};


