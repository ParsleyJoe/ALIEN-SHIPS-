#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include "bulletSpawning.hpp"
#include "gameAssets.hpp"
#include "hud.hpp"
#include "spawning.hpp"

enum class EnemyType {
	FODDER, SHIP, SINWAVE_SHOOTER, ASTEROID, BOSS_SHIP, RADIATING_SHOOTER
};

struct EnemyUpdateContext 
{
	std::vector<std::unique_ptr<Enemy>>& enemies;
};
// Using inheritance for different enemies
struct Enemy
{
	EnemyType type = EnemyType::FODDER;
	Rectangle rec = { 300, 100, 25, 25 };
	int health = 30;

	bool alive = true;
	virtual void Draw() = 0;
	virtual void Update(EnemyUpdateContext& ctx) = 0;
};

struct Fodder : Enemy
{
	int speedX = 5;
	int speedY = 2;
	float sizeDivider = 1.3f;
	void Draw() override
	{
		DrawCircle(rec.x + (rec.width / 2), rec.y + (rec.height / 2), rec.width / sizeDivider, RED);
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, DARKGREEN);
	}
	void Update(EnemyUpdateContext& ctx) override
	{
		int rightBound = GetScreenWidth() - rec.width;
		if ((rec.x) >= rightBound)
		{
			rec.x = rightBound;
			speedX = -speedX;
		}
		else if (rec.x <= 0)
		{
			rec.x = 0;
			speedX = -speedX;
		}

		// Move
		rec.x += speedX;
		rec.y += speedY;
	}
	Fodder()
	{
		type = EnemyType::FODDER;
	}
};

struct Ship : Enemy
{
	BulletSpawner bltSpawner;
	int speed = 3;
	void Draw() override
	{
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, BLACK);
		DrawTexture(GameAssets::shipSprite, rec.x, rec.y, BLUE);
	}

	void Update(EnemyUpdateContext& ctx) override
	{
		rec.y += speed; // moving only on y
		bltSpawner.position.y += speed;
		SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, DEG2RAD * 0.0f);
	}
	Ship()
	{
		type = EnemyType::SHIP;
	}
};

struct SinwaveShooter : Enemy
{
	BulletSpawner bltSpawner;
	float shootAngle;
	float sinMultiplier = 0.0f;
	void Update(EnemyUpdateContext& ctx) override
	{
		// TODO: Move it, Haven't decided how I'll Move it....
		sinMultiplier = sinf(GetTime() * 10.0f); // amplify the curve speed by 10.0f
		SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, shootAngle * sinMultiplier);
	}

	void Draw() override
	{
		DrawRectangleRec(rec, BLUE);
	}
};

struct RadialShooter : Enemy
{
	int speed = 3;
	float angle = 36;
	BulletSpawner bltSpawner;
	int cooldown = 3.0f;
	void Update(EnemyUpdateContext& ctx) override
	{
		float timeUntilShoot = (bltSpawner.lastSpawned + cooldown) - GetTime();
		float timeSinceShot = (GetTime() - bltSpawner.lastSpawned);
		if (!(timeUntilShoot <= 1.0f) && (timeSinceShot >= 1.0f)) // If not about to shoot move
		{
			rec.y += speed;
		}

		if (GetTime() - bltSpawner.lastSpawned >= cooldown)
		{
			Shoot();
		}
	}

	void Draw()
	{
		DrawRectangleRec(rec, RED);
	}

	void Shoot()
	{
		for (int i = 0; i < 10; i++)
		{
			bltSpawner.position = Vector2{ rec.x, rec.y };
			SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, angle);
			bltSpawner.direction = Vector2Rotate(bltSpawner.direction, angle);
		}
		bltSpawner.lastSpawned = GetTime();
	}
};

struct BossShip : Enemy
{
	HealthBar healthBar;
	int startHealth;
	Spawner radialShooterSpawner;
	BossShip()
	{
		startHealth = 1500;
		health = startHealth;
		healthBar.mainRec = Rectangle{ GetScreenWidth() - 50.0f, 40.0f, 30.0f, GetScreenHeight() - 170.0f };
		healthBar.fillRec = Rectangle{ healthBar.mainRec.x + 3, healthBar.mainRec.y + 3, healthBar.mainRec.width - 6, healthBar.mainRec.height - 6 };

		radialShooterSpawner.waveType = EnemyType::RADIATING_SHOOTER;
		radialShooterSpawner.spawnInterval = 5.0f;
	}

	void Update(EnemyUpdateContext& ctx) override
	{
		healthBar.health = health;

		if (GetTime() - radialShooterSpawner.lastSpawnTime >= radialShooterSpawner.spawnInterval)
		{
			SpawnRadiatingShooter(ctx.enemies, radialShooterSpawner, rec);
			radialShooterSpawner.lastSpawnTime = GetTime();
		}
	}

	void Draw()
	{
		// Drawing the boss
		DrawRectangle(rec.x, rec.y, rec.width, rec.height, GREEN);

		// Drawing health bar and skull sprite
		DrawHealthBar(healthBar, startHealth);
		DrawTexture(GameAssets::bossSkullSprite, healthBar.mainRec.x, healthBar.mainRec.y + healthBar.mainRec.height + 10, WHITE);
	}
};

struct Asteroid : Enemy
{
	int speedX = 15;
	int speedY = 15;
	float sizeDivider = 1.4f;

	void Draw() override
	{
		DrawCircle(rec.x + (rec.width / 2), rec.y + (rec.height / 2), rec.width / sizeDivider, RED);
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GREEN);
	}

	void Update(EnemyUpdateContext& ctx) override
	{
		rec.x += speedX;
		rec.y += speedY;
	}
};
