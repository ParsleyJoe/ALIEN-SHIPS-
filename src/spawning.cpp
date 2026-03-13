#include <spawning.hpp>
#include <iostream>
#include "gameAssets.hpp"
#include "enemy.hpp"
#include "raylib.h"
#include <algorithm>
#include <memory>
#include <vector>


void InitSpawners(SpawnerHolder& holder)
{
	holder.fodderSpawner.enemyAmmount = 10;
	holder.fodderSpawner.waveType = EnemyType::FODDER;
	holder.fodderSpawner.spawnInterval = 0.6f;
	holder.fodderSpawner.spawnedEnemies = 0;

	holder.shipSpawner.waveType = EnemyType::SHIP;
	holder.shipSpawner.spawnInterval = 0.3f;
	holder.shipSpawner.enemyAmmount = 15;
	holder.sinwaveShooter.spawnedEnemies = 0;

	holder.sinwaveShooter.waveType = EnemyType::SINWAVE_SHOOTER;
	holder.sinwaveShooter.enemyAmmount = 1;
	holder.sinwaveShooter.spawnInterval = 2.0f;
	holder.sinwaveShooter.spawnedEnemies = 0;

	holder.bossSpawner.waveType = EnemyType::BOSS_SHIP;
	holder.bossSpawner.enemyAmmount = 1;
	holder.bossSpawner.spawnInterval = 4.0f;
	holder.bossSpawner.spawnedEnemies = 0;

	holder.asteroidSpawner.waveType = EnemyType::ASTEROID;
	holder.asteroidSpawner.spawnInterval = GetRandomValue(15, 25) + 0.0f;
	holder.asteroidSpawner.enemyAmmount = 1;
	holder.asteroidSpawner.spawnedEnemies = 0;

	holder.barrierSpawner.waveType = EnemyType::BARRIER;
	holder.barrierSpawner.spawnInterval = 5.0f;
	holder.barrierSpawner.enemyAmmount = GetRandomValue(1, 3);
	holder.barrierSpawner.spawnedEnemies = 0;

	holder.creeperSpawner.waveType = EnemyType::CREEPER;
	holder.creeperSpawner.spawnedEnemies = 0;
	holder.creeperSpawner.enemyAmmount = 1;
	holder.creeperSpawner.spawnInterval = 1.0f;

}


void StartSpawning(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, SpawnerHolder& holder, Player* player)
{
	ClearEnemies(enemies);
	switch (wave)
	{
	case 1:
		SpawnEnemies(wave, enemies, holder.fodderSpawner);
		break;
	case 2:
		SpawnEnemies(wave, enemies, holder.shipSpawner);
		break;
	case 3:
		SpawnEnemies(wave, enemies, holder.sinwaveShooter);
		break;
	}
	SpawnAsteroid(enemies, holder.asteroidSpawner);
	SpawnBarrier(enemies, holder);
	SpawnCreeper(enemies, holder.creeperSpawner, player);
}

void SpawnEnemies(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
{
	int count = 0;
	for (auto& e : enemies)
	{
		if (e->type != EnemyType::BARRIER && e->type != EnemyType::ASTEROID)
			count++;
	}

	float gameTime = GetTime() - GameAssets::gameStartTime;
	if (((gameTime - spawner.lastSpawnTime) >= spawner.spawnInterval) && spawner.spawnedEnemies < spawner.enemyAmmount)
	{
		if (spawner.waveType == EnemyType::FODDER)
		{
			SpawnFodder(enemies);
			spawner.lastSpawnTime = gameTime;
		}
		else if (spawner.waveType == EnemyType::SHIP)
		{
			SpawnShips(enemies);
			spawner.lastSpawnTime = gameTime;
		}
		else if (spawner.waveType == EnemyType::SINWAVE_SHOOTER)
		{
			if (count <= 0)
				spawner.lastSpawnTime = gameTime;
			SpawnSinShooter(enemies, spawner);
		}
		else if (spawner.waveType == EnemyType::BOSS_SHIP)
		{
			if (count <= 0)
				spawner.lastSpawnTime = gameTime;
			SpawnBossShip(enemies);
		}

		spawner.spawnedEnemies++;
		count++;
		std::cout << "Enemy vector Size: " << enemies.size() << std::endl;
	}
	
	if (spawner.spawnedEnemies >= spawner.enemyAmmount && count <= 0)
	{
		if (spawner.enemyAmmount == 1 && spawner.waveType == EnemyType::SINWAVE_SHOOTER)
		{
			spawner.enemyAmmount = 2;
		}

		spawner.spawnedEnemies = 0;// TODO: it sets to 0 as soon as enemyAmmount is greater
		wave++;

		if (wave >= 4) { wave = 1; } // Reset wave, start again from fodders
	}
}

void SpawnSinShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
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

	std::unique_ptr<SinwaveShooter> sinShooter = std::make_unique<SinwaveShooter>();
	sinShooter->type = EnemyType::SINWAVE_SHOOTER;
	sinShooter->shootAngle = DEG2RAD * 15.0f;
	sinShooter->rec.x = pos.x;
	sinShooter->rec.y = pos.y;
	sinShooter->bltSpawner = MakeSpawner(Vector2{ (pos.x + sinShooter->rec.width / 2), (pos.y + sinShooter->rec.height) }, shooterCooldown);
	enemies.push_back(std::move(sinShooter));
}

void SpawnShips(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	// Generate Position
	Vector2 pos = GetShipSpawnPosition(enemies);

	// Creating enemy
	std::unique_ptr<Ship> ship = std::make_unique<Ship>();
	ship->rec.x = pos.x;
	ship->rec.y = pos.y;
	ship->rec.width = 37; // !So that rectangle fits with sprite 37, 20 are needed
	ship->rec.height = 20;
	ship->bltSpawner = MakeSpawner(Vector2{ (pos.x + (ship->rec.width / 2.0f)) - GameAssets::bullet.width,
		ship->rec.y + ship->rec.height }, GetRandomValue(1, 3) / 10.0f);
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

void SpawnCreeper(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner, Player* player)
{
	float gameTime = GetTime() - GameAssets::gameStartTime;
	if (gameTime - spawner.lastSpawnTime >= spawner.spawnInterval)
	{
		std::unique_ptr<Creeper> creeper = std::make_unique<Creeper>();
		creeper->rec.x = 300;
		creeper->rec.y = 300;
		creeper->player = player;
		enemies.push_back(std::move(creeper));
		spawner.lastSpawnTime = gameTime;
	}
}

void SpawnBossShip(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	std::unique_ptr<BossShip> boss = std::make_unique<BossShip>();
	boss->type = EnemyType::BOSS_SHIP;
	boss->rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, 50, 100, 100 };

	enemies.push_back(std::move(boss));
}

// NOTE: The enemies that boss spawns
void SpawnRadialShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner, Rectangle& rec)
{
	std::unique_ptr<RadialShooter> radiatingShooter = std::make_unique<RadialShooter>();
	radiatingShooter->rec = Rectangle{ rec.x + (rec.width / 2.0f), rec.y + rec.height + 5.0f, 20, 20 };
	radiatingShooter->health = 100;
	radiatingShooter->type = EnemyType::RADIATING_SHOOTER;
	radiatingShooter->bltSpawner = MakeSpawner(Vector2{ rec.x, rec.y }, 0.0f);

	enemies.push_back(std::move(radiatingShooter));
}

void SpawnAsteroid(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner)
{
	Vector2 spawnPos = { 0, 0 };
	spawnPos.x = GetRandomValue(0, 1) == 0 ? 0 : GetScreenWidth();

	float gameTime = GetTime() - GameAssets::gameStartTime;
	float timeUntilTrue = (spawner.lastSpawnTime + spawner.spawnInterval) - gameTime;
	if (timeUntilTrue <= 2.0f)
	{
		// Spawn in 1 second
		AsteroidWarning();
	}
	if (((gameTime - spawner.lastSpawnTime) >= spawner.spawnInterval))
	{
		std::unique_ptr<Asteroid> asteroid = std::make_unique<Asteroid>();
		asteroid->health = 300;
		asteroid->rec.width = 250;
		asteroid->rec.height = 250;
		if (spawnPos.x >= GetScreenWidth())
			asteroid->speedX *= -1;

		if (spawnPos.x >= GetScreenWidth())
		{
			asteroid->rec.x = spawnPos.x + asteroid->rec.width;
			asteroid->rec.y = spawnPos.y - asteroid->rec.height * 1.5;
		}
		else
		{
			asteroid->rec.x = spawnPos.x - asteroid->rec.width;
			asteroid->rec.y = spawnPos.y - asteroid->rec.height;
		}

		asteroid->type = EnemyType::ASTEROID;
		enemies.push_back(std::move(asteroid));
		spawner.lastSpawnTime = gameTime;
		spawner.spawnInterval = GetRandomValue(15, 25) + 0.0f;
	}
}

void AsteroidWarning()
{
	int fontSize = 30;
	int textWidth = MeasureText("GAME OVER", fontSize);
	int x = (GetScreenWidth() / 2) - (textWidth / 2);
	int y = (GetScreenHeight() / 2) - (fontSize / 2) - 200; // optional, for vertical center
	DrawText("Asteroid WARNING!!", x, y, fontSize, DARKGRAY); // !? Haven't started BeginDrawing() yet when this is called may cause issues
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

void SpawnBarrier(std::vector<std::unique_ptr<Enemy>>& enemies, SpawnerHolder& holder)
{

	bool barrierExists = std::any_of(enemies.begin(), enemies.end(), [](const auto& e){
		return e->type == EnemyType::BARRIER;
	});

	float gameTime = GetTime() - GameAssets::gameStartTime;
	if (gameTime < holder.barrierStartSpawnTime)
	{
		return;
	}
	
	if (!barrierExists && gameTime - holder.barrierSpawner.lastSpawnTime >= holder.barrierSpawner.spawnInterval)
	{
		std::cout << "Spawned Barrier" << std::endl;
		std::unique_ptr<Barrier> barrier = std::make_unique<Barrier>();
		holder.barrierSpawner.lastSpawnTime = gameTime;
		enemies.push_back(std::move(barrier));
	}
}
void ClearEnemies(std::vector<std::unique_ptr<Enemy>>& enemies)
{
	enemies.erase(std::remove_if(enemies.begin(), enemies.end(), [](const auto& e) {
		return !e ||
			e->rec.y > GetScreenHeight() + e->rec.height ||
			!e->alive;
	}), enemies.end());	
}
