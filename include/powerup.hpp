#pragma once
#include <raylib.h>

struct Player;

enum PowerUpType {
	SPEED = 0, POWER = 1, ONEUP = 2, SHIELD = 3
};

struct PowerUp {
	Rectangle rec;
	PowerUpType type;

	bool used = false;
	Texture2D sprite;
};

struct PowerUpEffect {
	PowerUpType type;
	float timeLeft;
	float totalTime = 5.0f;
};

void SpawnPowerUp(Vector2 pos);
void DrawPowerUps();
void PowerUpCollision(Player& player);
