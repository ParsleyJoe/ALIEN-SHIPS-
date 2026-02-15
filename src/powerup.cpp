#include "gameAssets.hpp"
#include "player.hpp"
#include <algorithm>
#include <iostream>
#include <powerup.hpp>
#include <raylib.h>

void SpawnPowerUp(Vector2 pos)
{
	PowerUpType type = (GetRandomValue(0, 1) == 0) ? PowerUpType::SPEED : PowerUpType::POWER;
	PowerUp powerUp = {.rec = {pos.x, pos.y, 30.0f, 30.0f}, .type = type};
	GameAssets::powerUps.push_back(powerUp);
}

void DrawPowerUps()
{
	// NOTE: PlaceHolder for sprites
	for (auto p : GameAssets::powerUps) {
		DrawRectangleRec(p.rec, GREEN);
	}
}

void PowerUpCollision(Player& player)
{
	for (auto& p : GameAssets::powerUps)
	{
		if (CheckCollisionRecs(player.rec, p.rec))
		{
			PerformPowerUp(player, p);
			p.used = true;
			std::cout << "PowerUPCollision" << '\n';
		}
	}

	GameAssets::powerUps.erase(std::remove_if(GameAssets::powerUps.begin(), GameAssets::powerUps.end(), [](const auto& p){
		return p.used;
	}), GameAssets::powerUps.end());
}
