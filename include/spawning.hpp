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
	int spawnedEnemies = 0;
};

struct SpawnerHolder
{
	Spawner fodderSpawner;
	Spawner shipSpawner;
	Spawner sinwaveShooter;
	Spawner bossSpawner;
	Spawner asteroidSpawner;
	Spawner barrierSpawner;
};

// Initiate spawner structs
void InitSpawners(SpawnerHolder& holder);

// Initiate enemy spawning, called everyframe
void StartSpawning(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, SpawnerHolder& holder);

// Helper Function for clearing enemy vector
void ClearEnemies(std::vector<std::unique_ptr<Enemy>>& enemies);

// Main function that calls other enemy specific functions
void SpawnEnemies(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);

void SpawnSinShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);
void SpawnShips(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnFodder(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnAsteroid(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);

void SpawnBossShip(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnRadialShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner, Rectangle& rec);

void AsteroidWarning();
Vector2 GetShipSpawnPosition(std::vector<std::unique_ptr<Enemy>>& enemies);

void SpawnBarrier(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);
