#include "spawning.hpp"
#include "gameState.hpp"
#include <algorithm>

void SpawnEnemies(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
{
	static int spawnedEnemies = 0;
	if (((GetTime() - spawner.lastSpawnTime) >= spawner.spawnInterval) && spawnedEnemies < spawner.enemyAmmount)
	{
		if (spawner.waveType == EnemyType::FODDER)
		{
			SpawnFodder(enemies);
			spawner.lastSpawnTime = GetTime();
		}
		else if (spawner.waveType == EnemyType::SHIP)
		{
			SpawnShips(enemies);
			spawner.lastSpawnTime = GetTime();
		}
		else if (spawner.waveType == EnemyType::CIRCLESHOOTER)
		{
       		if (enemies.size() <= 0)
				spawner.lastSpawnTime = GetTime();
			SpawnCircleShooter(enemies, spawner);
		}

		spawnedEnemies++;
	}

	if (spawnedEnemies >= spawner.enemyAmmount && enemies.size() <= 0)
	{
		if (spawner.enemyAmmount == 1)
		{
			spawner.enemyAmmount = 2;
		}
		if (spawner.waveType == EnemyType::CIRCLESHOOTER) { wave = 1; }
		spawnedEnemies = 0;// TODO: it sets to 0 as soon as enemyAmmount is greater
		wave++;
	}
}

void SpawnCircleShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
{
	float shooterCooldown = 0.1f;
	
	static Vector2 pos;
	static float posOffset = 250;
	pos = { GetScreenWidth() / 2.0f, 50.0f };
	if (spawner.enemyAmmount == 2)
	{
		Vector2 newPos = pos;
		do
		{
  			newPos.x -= posOffset;
			posOffset = posOffset * -1;
		} while (pos.x == newPos.x);
		pos = newPos;
	}

	std::unique_ptr<CircleShooter> circleShooter = std::make_unique<CircleShooter>();
	circleShooter->type = EnemyType::CIRCLESHOOTER;
	circleShooter->shootAngle = DEG2RAD * 15.0f;
	circleShooter->rec.x = pos.x;
	circleShooter->rec.y = pos.y;
	circleShooter->bltSpawner = MakeSpawner(Vector2{ (pos.x + circleShooter->rec.width / 2), (pos.y + circleShooter->rec.height) }, shooterCooldown);
	enemies.push_back(std::move(circleShooter));
}

void SpawnShips(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	// Generate Position
	Vector2 pos = GetShipSpawnPosition(enemies);

	// Creating enemy
	std::unique_ptr<Enemy> ship = std::make_unique<Ship>();
	ship->rec.x = pos.x;
	ship->rec.y = pos.y;
	ship->type = EnemyType::SHIP;
	enemies.push_back(std::move(ship));
}

void SpawnFodder(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	static Vector2 spawnPos = { -1, 100 };
	if (spawnPos.x == -1)
	{
		spawnPos.x = (GetRandomValue(0, 1) == 0) ? GetScreenWidth() : 0; // Random Side of the screen
	}

	std::unique_ptr<Enemy> fodder = std::make_unique<Fodder>();
	if (spawnPos.x == GetScreenWidth())
		fodder->rec.x = GetScreenWidth() - fodder->rec.width;
	else
		fodder->rec.x = spawnPos.x;
	fodder->rec.y = spawnPos.y;
	fodder->type = EnemyType::FODDER;
	enemies.push_back(std::move(fodder));
}

void SpawnAsteroid(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
{
	Vector2 spawnPos = { 0, 0 };
	spawnPos.x = GetRandomValue(0, 1) == 0 ? 0 : GetScreenWidth();

	if (((GetTime() - spawner.lastSpawnTime) >= spawner.spawnInterval))
	{
		std::unique_ptr<Asteroid> asteroid = std::make_unique<Asteroid>();
		asteroid->health = 300;
		asteroid->rec.x = spawnPos.x;
		asteroid->rec.y = spawnPos.y;
		asteroid->rec.width = 250;
		asteroid->rec.height = 250;
		if (asteroid->rec.x >= GetScreenWidth())
			asteroid->speedX *= -1;
		asteroid->type = EnemyType::ASTEROID;
		enemies.push_back(std::move(asteroid));
		spawner.lastSpawnTime = GetTime();
	}
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
		return e->rec.y > (GetScreenHeight() + e->rec.height);
		}), enemies.end());
}

Vector2 GetShipSpawnPosition(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	Vector2 pos;
	bool positionTaken = false;
	Ship enemy; // for enemy.rec.height
	do
	{
		pos = { 0.0f + GetRandomValue(1, GetScreenWidth() + enemy.rec.height), 100.0f };
		positionTaken = std::any_of(enemies.begin(), enemies.end(), [&pos](const auto& e) {
			return CheckCollisionRecs(Rectangle{ pos.x, pos.y, e->rec.width + 5, e->rec.height + 5 }, e->rec);
			});
		// If collision turns true then the position is around a enemy, we redo the RandomGeneration
	} while (positionTaken); // Checks if any enemy has the position 'pos'

	return pos;
}