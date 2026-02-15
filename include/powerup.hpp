#pragma once
#include <raylib.h>

struct Player;

enum class PowerUpType {
	SPEED, POWER
};

struct PowerUp {
	Rectangle rec;
	PowerUpType type;

	bool used = false;
};

void SpawnPowerUp(Vector2 pos);
void DrawPowerUps();
void PowerUpCollision(Player& player);
