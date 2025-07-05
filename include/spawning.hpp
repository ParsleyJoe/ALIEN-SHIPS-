#pragma once
#include <vector>
#include <memory>
#include <raylib.h>

struct Enemy;

enum class EnemyType;

struct Spawner
{
	float spawnInterval = 5.0f;
	float lastSpawnTime = 0.0f;
	EnemyType waveType;
	int enemyAmmount = 10;
};

void SpawnEnemies(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);

void SpawnCircleShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);
void SpawnShips(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnFodder(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnAsteroid(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);

void SpawnBossShip(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnRadiatingShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner, Rectangle& rec);

void AsteroidWarning();
Vector2 GetShipSpawnPosition(std::vector<std::unique_ptr<Enemy>>& enemies);