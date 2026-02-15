#pragma once
#include <raylib.h>

enum class PowerUpType {
	SPEED, POWER
};

struct PowerUp {
	Vector2 pos;
	PowerUpType type;
};

void SpawnPowerUp(Vector2 pos);
void DrawPowerUps();
