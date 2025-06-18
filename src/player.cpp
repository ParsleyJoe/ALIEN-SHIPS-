#include "player.hpp"
#include "enemy.hpp"
#include "raymath.h"
#include <algorithm>
#include <iostream>

// If Enemy Collided with player
void PlayerCollision(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	for (auto& enemy : enemies)
	{
		if (CheckCollisionRecs(player.rec, enemy->rec) && (GetTime() - player.lastHit) >= player.hitCooldown)
		{
			player.lives--;
			PlayerRestart(player);
		}
	}
}

void DrawPlayer(Player& player, Texture2D& playerSprite)
{
	DrawRectangleLines(player.rec.x, player.rec.y, player.rec.width, player.rec.height, RED);
	DrawTexture(playerSprite, player.rec.x, player.rec.y, WHITE);
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

void ShootBullet(Player& player, Rectangle bullet, std::vector<Rectangle>& bullets)
{

	if (IsKeyDown(KEY_SPACE) && (GetTime() - player.lastShot) >= player.shotCooldown)
	{
		bullet.x = player.rec.x + ((player.rec.width / 2.0f) * 0.5f) - (bullet.width / 2.0f);
		bullet.y = player.rec.y - bullet.height;
		bullets.push_back(bullet); // Add the bullet to the vector
		bullet.x += ((player.rec.width / 2.0f) * 1.0f);
		bullets.push_back(bullet);
		player.lastShot = GetTime();
	}
	for (Rectangle& bullet : bullets) // Iterate through the bullets
	{
		if (bullet.y < -bullet.height) // If the bullet goes off the screen
		{
			bullets.erase(std::remove_if(bullets.begin(), bullets.end(), [](const Rectangle& rec) {
				return rec.y < -rec.height; // Check if the bullet is off the screen
				}), bullets.end()); // Remove it from the vector
		}
		else
		{
			bullet.y -= 17; // Move the bullet up
		}
	}
}

// If playerBullets hit enemies
void BulletsCollision(std::vector<Rectangle>& playerBullets, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	for (const Rectangle& bullet : playerBullets)
	{
		for (auto& enemy : enemies)
		{
			if (enemy->alive && CheckCollisionRecs(bullet, enemy->rec))
			{
				if (enemy->health <= 0)
					enemy->alive = false;
				else
					enemy->health -= 20;
				playerBullets.erase(std::remove_if(playerBullets.begin(), playerBullets.end(), [&enemy](const Rectangle& rec) {
					return CheckCollisionRecs(rec, enemy->rec); // Check if the bullet collides with the enemy
					}), playerBullets.end()); // Remove the bullet from the vector
			}
		}
	}

	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
		return (e->rec.y > GetScreenHeight()) || !e->alive;
		}), enemies.end());
}


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