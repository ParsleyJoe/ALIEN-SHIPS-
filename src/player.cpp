#include "player.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include "raymath.h"
#include <algorithm>

// Load Player Textures
void pLoadTxt(Player& player)
{
	player.playerSprite = LoadTexture("resources/ship.png");
	player.bulletSprite = LoadTexture("resources/bullet.png");
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
	if (CheckCollisionRecs(player.rec, enemy->rec))
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
			player.lives--;
			PlayerRestart(player);
		}
	}
}

void DrawPlayer(Player& player)
{
	DrawRectangleLines(player.rec.x, player.rec.y, player.rec.width, player.rec.height, RED);

	if (IsTextureReady(player.playerSprite))
		DrawTexture(player.playerSprite, player.rec.x, player.rec.y, WHITE);
}

void MovePlayer(Player& player)
{
	if (!player.active)
	{
		player.active = PlayerStartAnimation(player);
		return;
	}

	if (IsKeyDown(KEY_A) && player.rec.x >= 0)
		player.direction.x = -1;
	if (IsKeyDown(KEY_D) && player.rec.x <= (GetScreenWidth() - player.rec.width))
		player.direction.x = 1;
	if (IsKeyDown(KEY_W) && player.rec.y >= player.rec.height)
		player.direction.y = -1;
	if (IsKeyDown(KEY_S) && player.rec.y <= (GetScreenHeight() - player.rec.height))
		player.direction.y += 1;

	if (Vector2Length(player.direction) > 0)
		player.direction = Vector2Normalize(player.direction);

	player.rec.x += player.direction.x * player.speed;
	player.rec.y += player.direction.y * player.speed;
	player.direction = { 0, 0 };
}

void ShootBullet(Player& player, Rectangle bullet)
{
	if (IsKeyDown(KEY_SPACE) && (GetTime() - player.lastShot) >= player.shotCooldown)
	{
		Bullet blt;
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
void BulletsHit(std::vector<Bullet>& playerBullets, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	for (const Bullet& bullet : playerBullets)
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
		playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(), [&enemy](const Bullet& blt) {
			return CheckCollisionRecs(blt.rec, enemy->rec); // Check if the bullet collides with the enemy
		}), playerBullets.end()); // Remove the bullet from the vector
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
		return (e->rec.y > GetScreenHeight()) || !e->alive;
		}), enemies.end());
}

// ran when player dies 
void PlayerRestart(Player& player)
{
	player.rec.x = (GetScreenWidth() / 2) - player.rec.width;
	player.rec.y = GetScreenHeight();
	player.active = PlayerStartAnimation(player);
	player.lastHit = GetTime();
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
