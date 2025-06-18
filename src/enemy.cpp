#include "enemy.hpp"
#include <algorithm>
#include <bulletSpawning.hpp>

void EnemyShootBullets(std::vector<Bullet>& enemyBullets, std::vector<std::unique_ptr<Enemy>>& enemies, Rectangle bulletTemplate)
{
	// "Instantiate" bullets
	for (auto& enemy : enemies)
	{
		if (enemy->type != EnemyType::SHIP)
		{
			continue;
		}
		if (enemy->time > enemy->shotInterval)
		{
			// Shoot
			bulletTemplate.x = enemy->rec.x + (enemy->rec.width / 2);
			bulletTemplate.y = enemy->rec.y + enemy->rec.height;
			Bullet blt;
			blt.rec = bulletTemplate;
			blt.dir = { 0, 1 };
			enemyBullets.push_back(blt);
			enemy->time = 0.0f;
		}
		else
		{
			enemy->time += GetFrameTime();
		}
	}


	enemyBullets.erase(std::remove_if(enemyBullets.begin(), enemyBullets.end(), [](const auto& b) {
		return b.rec.y > (GetScreenHeight() + b.rec.height);
		}), enemyBullets.end());
}
