#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include "bulletSpawning.hpp"
#include "gameState.hpp"

enum class EnemyType {
	FODDER, SHIP, CIRCLESHOOTER, ASTEROID
};

// Using inheritance for different enemies
struct Enemy
{
	EnemyType type = EnemyType::FODDER;
	Rectangle rec = { 300, 100, 25, 25 };
	int health = 30;

	bool alive = true;
	float shotInterval = (GetRandomValue(1, 3)) / 10.0f;// divide by 10.0f , to avoid integer division and truncation for 0.1, 0.3
	float time = shotInterval; // So that it fires as it spawns
	virtual void Draw() = 0;
	virtual void Update() = 0;
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
	void Update() override
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
	int speed = 3;
	void Draw() override
	{
		DrawRectangleRec(rec, BLUE);
	}
	void Update() override
	{
		rec.y += speed; // moving only on y 
	}
	Ship()
	{
		type = EnemyType::SHIP;
	}
};

struct CircleShooter : Enemy
{
	BulletSpawner bltSpawner;
	float shootAngle;
	float sinMultiplier = 0.0f;
	void Update() override
	{
		// TODO: Move it, Haven't decided how I'll Move it....
		sinMultiplier = sinf(GetTime() * 10.0f); // amplify the curve speed by 10.0f
		SpawnBullets(bltSpawner, GameState::enemyBullets, Rectangle{ 0, 0, 5, 10 }, shootAngle * sinMultiplier);
	}

	void Draw() override
	{
		DrawRectangleRec(rec, BLUE);
	}
};

struct Asteroid : Enemy
{
	int speedX = 15;
	int speedY = 15;
	float sizeDivider = 1.3f;

	void Draw() override
	{
		DrawCircle(rec.x + (rec.width / 2), rec.y + (rec.height / 2), rec.width / sizeDivider, RED);
	}

	void Update() override
	{
		rec.x += speedX;
		rec.y += speedY;
	}
};

// Shooting
void EnemyShootBullets(std::vector<Bullet>& enemyBullets, std::vector<std::unique_ptr<Enemy>>& enemies, Rectangle bullet);
