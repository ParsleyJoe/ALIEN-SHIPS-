#include "gameAssets.hpp"
#include "player.hpp"
#include <algorithm>
#include <iostream>
#include <memory>
#include <powerup.hpp>
#include <raylib.h>

void SpawnPowerUp(Vector2 pos)
{
	PowerUpType type = (PowerUpType)(GetRandomValue(0, 2));

	PowerUp powerUp = {.rec = {pos.x, pos.y, 30.0f, 30.0f}, .type = type};
	switch (type) {
	case PowerUpType::SPEED:
		powerUp.sprite = GameAssets::powerUpSprites["Speed"];
		break;
	case PowerUpType::POWER:
		powerUp.sprite = GameAssets::powerUpSprites["Power"];
		break;
	case PowerUpType::ONEUP:
		powerUp.sprite = GameAssets::powerUpSprites["OneUp"];
		break;
	case PowerUpType::SHIELD:
		std::cout << "Shield Sprite not loaded [powerup.cpp]\n";
		break;
	}
	GameAssets::powerUps.push_back(powerUp);
}

void DrawPowerUps()
{
	// NOTE: PlaceHolder for sprites
	for (auto p : GameAssets::powerUps) {
		if (IsTextureReady(p.sprite))
			DrawTexturePro(p.sprite, {0.0f, 0.0f, static_cast<float>(p.sprite.width), static_cast<float>(p.sprite.height)}, p.rec, {0.0f}, 0.0f, WHITE);
		else
			DrawRectangleRec(p.rec, GREEN);
	}
}

void PowerUpCollision(Player& player)
{
	for (auto& p : GameAssets::powerUps)
	{
		if (CheckCollisionRecs(player.rec, p.rec))
		{
			AddPowerUpEffect(player, p);
			p.used = true;
			std::cout << "PowerUPCollision" << '\n';
		}
	}

	GameAssets::powerUps.erase(std::remove_if(GameAssets::powerUps.begin(), GameAssets::powerUps.end(), [](const auto& p){
		return p.used;
	}), GameAssets::powerUps.end());
}
