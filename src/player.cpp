#include "player.hpp"
#include "enemy.hpp"
#include "game.hpp"
#include "gameAssets.hpp"
#include "powerup.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <memory>

// Load Player Textures
void pLoadTxt(Player& player)
{
	player.shipTextures[0] = LoadTexture("resources/ship.png");
	player.shipTextures[1] = LoadTexture("resources/rollingship.png");
	player.shipTextures[2] = LoadTexture("resources/extrabulletsship.png");
	player.bulletSprite = LoadTexture("resources/bullet.png");
}

void UpdatePlayer(Game& game, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	MovePlayer(game.player);
	PlayerCollision(game.player, enemies);

	ShootBullet(game.player, GameAssets::bullet);

	BulletsHit(game, enemies);
	CheckEffects(game.player);

	SpecialAbility(game.player);
}


// If Enemy Collided with player
void PlayerCollision(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	for (auto& enemy : enemies)
	{
		if ((GetTime() - player.lastHit) >= player.hitCooldown)
		{
			ContactCollision(player, enemy);
			BulletCollision(player, enemy);
		}
	}
}

void ContactCollision(Player& player, std::unique_ptr<Enemy>& enemy)
{
	if (!player.shieldActive && CheckCollisionRecs(player.rec, enemy->rec))
	{
		player.lives--;
		PlayerRestart(player);
	}
}

void BulletCollision(Player& player, std::unique_ptr<Enemy>& enemy)
{
	for (Bullet& blt : GameAssets::enemyBullets)
	{
		if (CheckCollisionRecs(player.rec, blt.rec))
		{
			if (!player.shieldActive)
			{
				player.lives--;
				PlayerRestart(player);
			}
			blt.rec = {GetScreenWidth() + blt.rec.width, GetScreenHeight() + blt.rec.height, 0.0f, 0.0f};
		}
	}
}

void DrawPlayer(Player& player)
{
	DrawRectangleLines(player.rec.x, player.rec.y, player.rec.width, player.rec.height, RED);

	DrawTexture(player.shipTextures[player.selectedShipIndex], player.rec.x, player.rec.y, WHITE);
}

void MovePlayer(Player& player)
{
	if (!player.active)
	{
		if (!player.specialActive)
			player.active = PlayerStartAnimation(player);
		return;
	}

	if (IsKeyDown(KEY_A) && player.rec.x >= 0)
		player.movDir.x = -1;
	if (IsKeyDown(KEY_D) && player.rec.x <= (GetScreenWidth() - player.rec.width))
		player.movDir.x = 1;
	if (IsKeyDown(KEY_W) && player.rec.y >= player.rec.height)
		player.movDir.y = -1;
	if (IsKeyDown(KEY_S) && player.rec.y <= (GetScreenHeight() - player.rec.height))
		player.movDir.y += 1;

	if (Vector2Length(player.movDir) > 0)
		player.movDir = Vector2Normalize(player.movDir);

	player.rec.x += player.movDir.x * player.speed;
	player.rec.y += player.movDir.y * player.speed;
	player.dir = player.movDir;
	player.movDir = { 0, 0 };
}

void SpecialAbility(Player& player)
{
	if (player.selectedShipIndex == 0 || player.specialMeter < player.specialFullLevel && !player.specialActive)
	{
		return;
	}

	std::cout << "SpecailAbility Called" << std::endl;
	
	if (IsKeyPressed(KEY_R) && !player.specialActive)
		player.specialActive = true;

	if (player.specialActive)
	{
		switch (player.selectedShipIndex) 
		{
		case 0:
			std::cout << "This ship has no ability" << '\n';
			break;
		case 1:
			std::cout << "Dodge Roll" << '\n';
			if (!DodgeRoll(player))
			{
				player.specialActive = false;
				player.active = true;
			}
			std::cout << "Dodge Rolling ";
			break;
		case 2:
			std::cout << "Extra Shots" << '\n';
			break;
		}
		std::cout << "Switch Executed";
		player.specialMeter = 0;
	}
}

bool DodgeRoll(Player& player)
{
	static bool animationActive = false;
	static int dir = 1;

	float dodgeOffset = 70.0f;
	static float distMoved = 0.0f;
	int speed = 20;

	if (player.dir.x != 0)
		dir = player.dir.x;

	if (!animationActive)
	{
		player.active = false;
		animationActive = true;
		player.specialActive = true;
		player.specialMeter = 0;
		std::cout << "Animation Started" << std::endl;
		//dir = Vector2Normalize(player.direction).x;
		//std::cout << "direction: " << dir;
	}
	else
	{
		float movingDist = speed * dir;
		float x = player.rec.x;
		player.rec.x += movingDist;
		distMoved += std::abs(x - player.rec.x);
		
		if (distMoved >= dodgeOffset)
		{
			animationActive = false;
			distMoved = 0.0f;
			std::cout << "Animation Ended" << std::endl;
		}
	}
	return animationActive;
}

void ShootBullet(Player& player, Rectangle bullet)
{
	if (IsKeyDown(KEY_SPACE) && (GetTime() - player.lastShot) >= player.shotCooldown)
	{
		Bullet blt;
		blt.dir = {0, 1};
		blt.sprite = player.bulletSprite;
		blt.rec = bullet; // Reset to Give correct width and height
		blt.rec.x = player.rec.x + ((player.rec.width / 2.0f) * 0.5f) - (bullet.width / 2.0f);
		blt.rec.y = player.rec.y - bullet.height;
		blt.speed = -17;
		player.playerBullets.push_back(blt); // Add the bullet to the vector

		// Shoot two bullets
		blt.rec.x += ((player.rec.width / 2.0f) * 1.0f);
		player.playerBullets.push_back(blt);

		player.lastShot = GetTime();
	}

	// Remove Bullets out of bounds
	player.playerBullets.erase(
		std::remove_if(player.playerBullets.begin(), player.playerBullets.end(), [](const Bullet& blt) {
				return blt.rec.y < -blt.rec.height; // Check if the bullet is off the screen
			}), player.playerBullets.end()); // Remove it from the vector

}

// If playerBullets hit enemies
void BulletsHit(Game& game, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	for (const Bullet& bullet : game.player.playerBullets)
	{
		for (auto& enemy : enemies)
		{
			if (enemy->alive && CheckCollisionRecs(bullet.rec, enemy->rec))
			{
				if (enemy->health <= 0)
					enemy->alive = false;
				else
					enemy->health -= 20;
			}
		}
	}

	// NOTE: Have to run loop twice: playerBullets cannot be modified while its being iterated through
	for (auto& enemy : enemies)
	{
		if (!enemy->alive)
		{
			enemy->Die();
			gEnemyKilled(game, enemy);
			game.player.specialMeter += game.player.specialIncrement;
			if (game.player.specialMeter > game.player.specialFullLevel)
			{
				game.player.specialMeter = game.player.specialFullLevel;
			}
		}

		game.player.playerBullets.erase(std::remove_if(game.player.playerBullets.begin(), game.player.playerBullets.end(), [&enemy](const Bullet& blt) {
			return CheckCollisionRecs(blt.rec, enemy->rec); // Check if the bullet collides with the enemy
		}), game.player.playerBullets.end()); // Remove the bullet from the vector
	}


}

// ran when player dies 
void PlayerRestart(Player& player)
{
	player.rec.x = (GetScreenWidth() / 2.0f) - player.rec.width;
	player.rec.y = GetScreenHeight();
	player.active = PlayerStartAnimation(player);
	player.lastHit = GetTime();
	
	// Reset PowerUps
	player.activeEffects.clear();
	player.speed = player.baseSpeed;
	player.damage = player.baseDamage;
}

// Returns if animation is done
bool PlayerStartAnimation(Player& player)
{
	if (player.rec.y <= 520.0f)
	{
		return true;
	}
	player.rec.y -= 4.0f;
	return false;
}

void AddPowerUpEffect(Player& player, PowerUp powerUp)
{
	PowerUpEffect effect;
	effect = {.type = powerUp.type, .timeLeft = effect.totalTime};
	player.activeEffects.push_back(effect);
}

void CheckEffects(Player& player)
{
	for (auto& p : player.activeEffects) {
		switch (p.type) 
		{
		case PowerUpType::SPEED:
			player.speed = player.baseSpeed + 5;
			break;
		case PowerUpType::POWER:
			player.damage = player.baseDamage + 10;
			break;
		case PowerUpType::ONEUP:
			player.lives++;
			p.timeLeft = -1.0f;
			break;
		case PowerUpType::SHIELD:
			player.shieldActive = true;
			break;
		}
		p.timeLeft -= GetFrameTime();

		// Reset For TimeOver
		if (p.timeLeft <= 0.0f)
		{
			switch (p.type) 
			{
			case PowerUpType::SPEED:
				player.speed = player.baseSpeed;
				break;
			case PowerUpType::POWER:
				player.damage = player.baseDamage;
				break;
			case PowerUpType::SHIELD:
				player.shieldActive = false;
				break;
			case PowerUpType::ONEUP:
				// Empty Switch to silence warning
				break;
                        }
                }
	}
	player.activeEffects.erase(std::remove_if(player.activeEffects.begin(), player.activeEffects.end(), [](const auto& p){
		return (p.timeLeft <= 0);
	}), player.activeEffects.end());
}

void CycleSelectedShip(Player& player, int dir)
{
	player.selectedShipIndex += dir;

	if (player.selectedShipIndex < 0)
		player.selectedShipIndex = 0;

	if (player.selectedShipIndex >= player.shipTextures.size())
		player.selectedShipIndex = player.shipTextures.size() - 1;
}
