#pragma once
#include "raylib.h"
#include "enemy.hpp"
#include <vector>
#include <memory>

struct Player
{
	bool active = false;
	int lives = 3;

	int damage = 20;
	float hitCooldown = 2.0f;
	float lastHit = 0;
	float shotCooldown = 0.1f;
	float lastShot = 0;
	std::vector<Bullet> playerBullets;
	Rectangle rec = { (GetScreenWidth() / 2) - (25.0f), (float)GetScreenHeight(), 33, 33 };
	Texture2D playerSprite;
	Texture2D bulletSprite;

	int speed = 7;
	Vector2 direction = { 0 };
};

void pLoadTxt(Player& player);

void DrawPlayer(Player& player);

void UpdatePlayer(Player& player);
void MovePlayer(Player& player);
void ShootBullet(Player& player, Rectangle bullet);

void BulletsHit(std::vector<Bullet>& bullets, std::vector<std::unique_ptr<Enemy>>& enemies);
void PlayerCollision(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
void ContactCollision(Player& player, std::unique_ptr<Enemy>& enemy);
void BulletCollision(Player& player, std::unique_ptr<Enemy>& enemy);

void PlayerRestart(Player& player);
bool PlayerStartAnimation(Player& player);
