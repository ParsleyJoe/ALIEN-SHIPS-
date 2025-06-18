#include "bulletSpawning.hpp"
#include "enemy.hpp"
#include <vector>

void SpawnBullets(BulletSpawner& bltSpawner, std::vector<Bullet>& enemyBullets, Rectangle bullet, float angle)
{
	if ((GetTime() - bltSpawner.lastSpawned) >= bltSpawner.spawnCooldown)
	{
		Bullet blt;
		blt.rec = bullet;
		blt.rec.x = bltSpawner.position.x;
		blt.rec.y = bltSpawner.position.y;
		blt.dir = Vector2Rotate(bltSpawner.direction, angle);
		enemyBullets.push_back(blt);
		bltSpawner.lastSpawned = GetTime();
	}

	RemoveBullets(enemyBullets);
}

// Remove offscreen bullets
void RemoveBullets(std::vector<Bullet>& enemyBullets)
{
	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const auto& b) {
		return b.rec.y > (GetScreenHeight() + b.rec.height);
		}), enemyBullets.end());
}

BulletSpawner MakeSpawner(Vector2 pos, float cooldown)
{
	BulletSpawner spawner;
	spawner.position = pos;
	spawner.direction = { 0, 1 };
	spawner.spawnCooldown = cooldown;
	return spawner;
}