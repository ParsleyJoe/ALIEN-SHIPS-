#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Enemy;

struct Bullet
{
	int speed = 7;
	Vector2 dir;

	Rectangle rec;

	void Draw()
	{
		DrawRectangleRec(rec, BLACK);
	}

	void Update()
	{
 		dir = Vector2Normalize(dir);
		rec.x += dir.x * speed;
		rec.y += dir.y * speed;
	}
};

struct BulletSpawner
{
	Vector2 position;
	Vector2 direction;

	float spawnCooldown = 0.3f;
	float lastSpawned = 0;
};

void SpawnBullets(BulletSpawner& bltSpawner, std::vector<Bullet>& enemyBullets, Rectangle bullet, float angle);
void RemoveBullets(std::vector<Bullet>& enemyBullets);
BulletSpawner MakeSpawner(Vector2 pos, float cooldown);