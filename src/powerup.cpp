#include "gameAssets.hpp"
#include <powerup.hpp>
#include <raylib.h>

void SpawnPowerUp(Vector2 pos)
{
	PowerUpType type = (GetRandomValue(0, 1) == 0) ? PowerUpType::SPEED : PowerUpType::POWER;
	PowerUp powerUp = {.pos = pos, .type = type};
	GameAssets::powerUps.push_back(powerUp);
}

void DrawPowerUps()
{
	for (auto p : GameAssets::powerUps) {
		DrawRectangleRec({p.pos.x, p.pos.y, 30, 30}, WHITE);
	}
}
