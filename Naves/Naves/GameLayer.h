#pragma once

#include "Layer.h"
#include "Player.h"
#include "Background.h"
#include "Enemy.h"
#include "Projectile.h"
#include "Text.h"
#include "Audio.h" 
#include "Bomb.h"
#include "Coin.h"

#include <list>

class GameLayer : public Layer
{
public:
	GameLayer(Game* game);
	void init() override;
	void processControls() override;
	void update() override;
	void draw() override;
	void keysToControls(SDL_Event event);

	Background* background;
	int newEnemyTime = 0;
	Audio* audioBackground;

	// Jugadores
	Player* player1;
	Player* player2;
	Text* playerText1;
	Text* playerText2;

	// Jugador 1 controles
	bool controlShoot1 = false;
	int controlMoveY1 = 0;
	int controlMoveX1 = 0;

	// Jugador 2 controles
	bool controlShoot2 = false;
	int controlMoveY2 = 0;
	int controlMoveX2 = 0;

	// Vidas jugador 1
	Actor* backgroundLifes1;
	Text* textLifes1;

	// Vidas jugador 2
	Actor* backgroundLifes2;
	Text* textLifes2;

	// Puntos compartidos entre jugadores
	Actor* backgroundPoints;
	Text* textPoints;
	int points;

	// Listas de actores
	list<Enemy*> enemies;
	list<Projectile*> projectiles;
	list<Bomb*> bombs;
	list<Coin*> coins;
};


