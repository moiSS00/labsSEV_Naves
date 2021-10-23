#include "GameLayer.h"

GameLayer::GameLayer(Game* game)
	: Layer(game) {
	//llama al constructor del padre : Layer(renderer)
	init();
}

void GameLayer::init() {
	audioBackground = new Audio("res/musica_ambiente.mp3", true);
	audioBackground->play();

	points = 0;
	textPoints = new Text("hola", WIDTH * 0.92, HEIGHT * 0.04, game);
	textPoints->content = to_string(points);

	player = new Player(50, 50, game);
	background = new Background("res/fondo.png", WIDTH * 0.5, HEIGHT * 0.5, -1, game);
	backgroundPoints = new Actor("res/icono_puntos.png",
		WIDTH * 0.85, HEIGHT * 0.05, 24, 24, game);

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
	if (controlShoot) {
		Projectile* newProjectile = player->shoot();
		if (newProjectile != NULL) {
			projectiles.push_back(newProjectile);
		}
	}

	// Eje X
	if (controlMoveX > 0) {
		player->moveX(1);
	}
	else if (controlMoveX < 0) {
		player->moveX(-1);
	}
	else {
		player->moveX(0);
	}

	// Eje Y
	if (controlMoveY > 0) {
		player->moveY(1);
	}
	else if (controlMoveY < 0) {
		player->moveY(-1);
	}
	else {
		player->moveY(0);
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
		case SDLK_d: // derecha
			controlMoveX = 1;
			break;
		case SDLK_a: // izquierda
			controlMoveX = -1;
			break;
		case SDLK_w: // arriba
			controlMoveY = -1;
			break;
		case SDLK_s: // abajo
			controlMoveY = 1;
			break;
		case SDLK_SPACE: // dispara
			controlShoot = true;
			break;
		}
	}
	if (event.type == SDL_KEYUP) {
		int code = event.key.keysym.sym;
		// Levantada
		switch (code) {
		case SDLK_d: // derecha
			if (controlMoveX == 1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_a: // izquierda
			if (controlMoveX == -1) {
				controlMoveX = 0;
			}
			break;
		case SDLK_w: // arriba
			if (controlMoveY == -1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_s: // abajo
			if (controlMoveY == 1) {
				controlMoveY = 0;
			}
			break;
		case SDLK_SPACE: // dispara
			controlShoot = false;
			break;
		}
	}
}


void GameLayer::update() {
	background->update();

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

	// Actualizar jugador
	player->update();

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
		if (player->isOverlap(enemy)) {
			player->loseLife();
			if (player->lifes <= 0) {
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
		if (player->isOverlap(bomb)) {
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
		if (player->isOverlap(coin)) {
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
	
	// Dibujar jugador
	player->draw();

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

	SDL_RenderPresent(game->renderer); // Renderiza
}

