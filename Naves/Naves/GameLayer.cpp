#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	// Jugadores
	player1 = new Player(50, 50, game);
	player2 = new Player(50, 200, game);

	// Texto puntos
	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	// Texto vidas
	textLifes1 = new Text("3", WIDTH * 0.72, HEIGHT * 0.04, game);

	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);
	backgroundLifes1 = new Actor("res/corazon.png",
		WIDTH * 0.65, HEIGHT * 0.05, 44, 36, game);

	// Vaciar por si reiniciamos el juego
	projectiles.clear();
	enemies.clear();
	bombs.clear();
	coins.clear();
}

void GameLayer::processControls() {
	// obtener controles
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		keysToControls(event);
	}
	//procesar controles
	
	// Disparar

	// Jugador 1
	if (controlShoot1) {
		Projectile* newProjectile = player1->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}

	// Jugador 2
	if (controlShoot2) {
		Projectile* newProjectile = player2->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}

	// Eje X

	// Jugador 1
	if (controlMoveX1> 0) {
		player1->moveX(1);
	}
	else if (controlMoveX1 < 0) {
		player1->moveX(-1);
	}
	else {
		player1->moveX(0);
	}

	// Jugador 2
	if (controlMoveX2 > 0) {
		player2->moveX(1);
	}
	else if (controlMoveX2 < 0) {
		player2->moveX(-1);
	}
	else {
		player2->moveX(0);
	}

	// Eje Y

	// Jugador 1
	if (controlMoveY1 > 0) {
		player1->moveY(1);
	}
	else if (controlMoveY1 < 0) {
		player1->moveY(-1);
	}
	else {
		player1->moveY(0);
	}

	// Jugador 2
	if (controlMoveY2 > 0) {
		player2->moveY(1);
	}
	else if (controlMoveY2 < 0) {
		player2->moveY(-1);
	}
	else {
		player2->moveY(0);
	}
}

void GameLayer::keysToControls(SDL_Event event) {
	if (event.type == SDL_KEYDOWN) {
		int code = event.key.keysym.sym;
		// Pulsada
		switch (code) {
		case SDLK_ESCAPE:
			game->loopActive = false;
			break;
		case SDLK_1:
			game->scale();
			break;
		case SDLK_d: // derecha 1
			controlMoveX1 = 1;
			break;
		case SDLK_a: // izquierda 1
			controlMoveX1 = -1;
			break;
		case SDLK_w: // arriba 1 
			controlMoveY1 = -1;
			break;
		case SDLK_s: // abajo 1
			controlMoveY1 = 1;
			break;
		case SDLK_SPACE: // dispara 1
			controlShoot1 = true;
			break;
		case SDLK_RIGHT: // derecha 2
			controlMoveX2 = 1;
			break;
		case SDLK_LEFT: // izquierda 2
			controlMoveX2 = -1;
			break;
		case SDLK_UP: // arriba 2
			controlMoveY2 = -1;
			break;
		case SDLK_DOWN: // abajo 2
			controlMoveY2 = 1;
			break;
		case SDLK_RETURN: // dispara 2
			controlShoot2 = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha 1
			if (controlMoveX1 == 1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_a: // izquierda 1
			if (controlMoveX1 == -1) {
				controlMoveX1 = 0;
			}
			break;
		case SDLK_w: // arriba 1
			if (controlMoveY1 == -1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_s: // abajo 1
			if (controlMoveY1 == 1) {
				controlMoveY1 = 0;
			}
			break;
		case SDLK_SPACE: // dispara 1
			controlShoot1 = false;
			break;
		case SDLK_RIGHT: // derecha 2
			if (controlMoveX2 == 1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_LEFT: // izquierda 2
			if (controlMoveX2 == -1) {
				controlMoveX2 = 0;
			}
			break;
		case SDLK_UP: // arriba 2
			if (controlMoveY2 == -1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_DOWN: // abajo 2
			if (controlMoveY2 == 1) {
				controlMoveY2 = 0;
			}
			break;
		case SDLK_RETURN: // dispara 2
			controlShoot2 = false;
			break;
		}
	}
}


void GameLayer::update() {

	// Actualizar fondo
	background->update();

	// Actualizar jugadores
	player1->update();
	player2->update();

	// Generar enemigos
	newEnemyTime--;
	if (newEnemyTime <= 0) {
		int rX = (rand() % (600 - 500)) + 1 + 500;
		int rY = (rand() % (300 - 60)) + 1 + 60;
		enemies.push_back(new Enemy(rX, rY, game));
		newEnemyTime = 110;

		// Generar bombas
		if (bombs.size() < 3) { // Para limitar el número de bombas en el mapa
			int rXBomb = (rand() % 400) + 1;
			int rYBomb = (rand() % 300) + 1;
			bombs.push_back(new Bomb(rXBomb, rYBomb, game));
		}

		// Generar monedas
		if (coins.size() < 3) { // Para limitar el número de monedas en el mapa
			int rXCoin = (rand() % 400) + 1;
			int rYCoin = (rand() % 300) + 1;
			coins.push_back(new Coin(rXCoin, rYCoin, game));
		}
	}

	// Actualizar enemigos 
	for (auto const& enemy : enemies) {
		enemy->update();
	}

	// Actualizar proyectiles 
	for (auto const& projectile : projectiles) {
		projectile->update();
	}

	// Colisiones, Player - Enemy
	for (auto const& enemy : enemies) {
		if (player1->isOverlap(enemy)) {
			player1->loseLife();
			textLifes1->content = to_string(player1->lifes);
			if (player1->lifes <= 0) {
				init();
				return;
			}
		}
	}

	// Colisiones , Enemy - Projectile
	list<Enemy*> deleteEnemies;
	list<Projectile*> deleteProjectiles;

	for (auto const& projectile : projectiles) {
		if (projectile->isInRender() == false) {

			bool pInList = std::find(deleteProjectiles.begin(),
				deleteProjectiles.end(),
				projectile) != deleteProjectiles.end();

			if (!pInList) {
				deleteProjectiles.push_back(projectile);
			}
		}
	}

	for (auto const& enemy : enemies) {
		for (auto const& projectile : projectiles) {
			if (enemy->isOverlap(projectile)) {
				bool pInList = std::find(deleteProjectiles.begin(),
					deleteProjectiles.end(),
					projectile) != deleteProjectiles.end();

				if (!pInList) {
					deleteProjectiles.push_back(projectile);
				}

				// Control de vidas del enemigo 
				enemy->loseLife();
				if (enemy->lifes <= 0) {
					bool eInList = std::find(deleteEnemies.begin(),
						deleteEnemies.end(),
						enemy) != deleteEnemies.end();
					if (!eInList) {
						deleteEnemies.push_back(enemy);
					}
					points++;
					textPoints->content = to_string(points);
				}
			}
		}
	}

	// Colisiones , Player - Bomb
	list<Bomb*> deleteBombs;
	for (auto const& bomb : bombs) {
		if (player1->isOverlap(bomb)) {
			deleteBombs.push_back(bomb);
			for (auto const& enemy : enemies) {
				if (enemy->isInRender() == true) { // Para sumar puntos de los enemigos destruidos en pantalla
					points++; 
					textPoints->content = to_string(points);
				}
				deleteEnemies.push_back(enemy);
			}
		}
	}

	// Colisiones , Player - Coin
	list<Coin*> deleteCoins;
	for (auto const& coin : coins) {
		if (player1->isOverlap(coin)) {
			deleteCoins.push_back(coin);
			// Incrementamos puntos 
			points++;
			textPoints->content = to_string(points);
		}
	}

	// Fase de eliminación 

	// Eliminamos enemigos
	for (auto const& delEnemy : deleteEnemies) {
		enemies.remove(delEnemy);
	}
	deleteEnemies.clear();

	// Eliminamos proyectiles
	for (auto const& delProjectile : deleteProjectiles) {
		projectiles.remove(delProjectile);
		delete delProjectile;
	}
	deleteProjectiles.clear();

	// Eliminamos bombas
	for (auto const& delBomb : deleteBombs) {
		bombs.remove(delBomb);
	}
	deleteBombs.clear();

	for (auto const& delCoin : deleteCoins) {
		coins.remove(delCoin);
	}
	deleteCoins.clear();

	cout << "update GameLayer" << endl;
}

void GameLayer::draw() {

	// Dibujar fondo
	background->draw();
	
	// Dibujar jugadores
	player1->draw();
	player2->draw();

	// Dibujar enemigos
	for (auto const& enemy : enemies) {
		enemy->draw();
	}

	// Dibujar proyectiles
	for (auto const& projectile : projectiles) {
		projectile->draw();
	}

	// Dibujar bombas
	for (auto const& bomb : bombs) {
		bomb->draw();
	}

	// Dibujar monedas
	for (auto const& coin : coins) {
		coin->draw();
	}

	// Dibujar marcador de puntos
	textPoints->draw();
	backgroundPoints->draw();

	// Dibujar marcador de vidas
	textLifes1->draw();
	backgroundLifes1->draw();

	SDL_RenderPresent(game->renderer); // Renderiza
}

