#pragma once
#include "powerup.hpp"
#include "raylib.h"
#include "enemy.hpp"
#include <array>
#include <vector>
#include <memory>

struct Game;

struct Player
{
	bool active = false;
	int lives = 3;

	bool shieldActive = false;
	int baseDamage = 20;
	int damage = 20;
	float hitCooldown = 2.0f;
	float lastHit = 0;
	bool abilityAvailable = true;

	float shotCooldown = 0.1f;
	float lastShot = 0;
	std::vector<Bullet> playerBullets;

	Rectangle rec = { 500.0f, 350.0f, 33, 33 };
	Texture2D bulletSprite;

	int baseSpeed = 7;
	int speed = 7;
	Vector2 direction = { 0 };

	std::vector<PowerUpEffect> activeEffects;

	int selectedShipIndex = 0;
	std::array<Texture2D, 3> shipTextures;
};

void pLoadTxt(Player& player);

void DrawPlayer(Player& player);

void UpdatePlayer(Game& game, std::vector<std::unique_ptr<Enemy>>& enemies);
void MovePlayer(Player& player);
void ShootBullet(Player& player, Rectangle bullet);

void AddPowerUpEffect(Player& player, PowerUp powerUp);
void CheckEffects(Player& player);

void SpecialAbility(Player& player);

void BulletsHit(Game& game, std::vector<std::unique_ptr<Enemy>>& enemies);
void PlayerCollision(Player& player, std::vector<std::unique_ptr<Enemy>>& enemies);
void ContactCollision(Player& player, std::unique_ptr<Enemy>& enemy);
void BulletCollision(Player& player, std::unique_ptr<Enemy>& enemy);

void PlayerRestart(Player& player);
bool PlayerStartAnimation(Player& player);

void CycleSelectedShip(Player& player, int dir);
