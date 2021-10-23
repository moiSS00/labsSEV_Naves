#include "Player.h"

Player::Player(float x, float y, Game* game)
	: Actor("res/jugador.png", x, y, 50, 57, game) {
	audioShoot = new Audio("res/efecto_disparo.wav", false);
}

void Player::update() {
	if (invulnerableTime > 0) {
		invulnerableTime--;
	}
	if (shootTime > 0) {
		shootTime--;
	}
	x = x + vx;
	y = y + vy;
}

void Player::moveX(float axis) {
	vx = axis * 3;
}

void Player::moveY(float axis) {
	vy = axis * 3;
}

Projectile* Player::shoot() {
	if (shootTime == 0) {
		audioShoot->play();
		shootTime = shootCadence;
		return new Projectile(x, y, game);
	}
	else {
		return NULL;
	}
}

void Player::loseLife() {
	if (invulnerableTime <= 0) {
		if (lifes > 0) {
			lifes--;
			invulnerableTime = 100; // El jugador es invlunerable durante 100 actualizaciones 	
		}
	}
}

void Player::draw() {
	if (invulnerableTime == 0) {
		Actor::draw(); // llamar al metodo del hijo
	}
	else {
		if (invulnerableTime % 10 >= 0 && invulnerableTime % 10 <= 5) {
			Actor::draw(); // llamar al metodo del hijo
		}
	}
}