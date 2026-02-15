#pragma once
#include <raylib.h>
#include <raymath.h>
#include <vector>

struct Enemy;

struct Bullet
{
	int speed = 7;
	Vector2 dir = {1, 1};

	Rectangle rec;
	Texture2D sprite;

	void Draw()
	{
		if (IsTextureReady(sprite))
			DrawTexture(sprite, rec.x, rec.y, WHITE);
		else
			DrawRectangleRec(rec, BLACK);

		//DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GREEN);
	}

	void Update()
	{
		dir = Vector2Normalize(dir);
		rec.y += dir.y * speed;
		rec.x += dir.x * speed;
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
