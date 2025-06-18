#pragma once
#include <vector>
#include <memory>
#include "enemy.hpp"

struct Spawner
{
	float spawnInterval = 5.0f;
	float lastSpawnTime = 0.0f;
	EnemyType waveType = EnemyType::FODDER;
	int enemyAmmount = 10;
};

void SpawnEnemies(int& wave, std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);

void SpawnCircleShooter(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);
void SpawnShips(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnFodder(std::vector<std::unique_ptr<Enemy>>& enemies);
void SpawnAsteroid(std::vector<std::unique_ptr<Enemy>>& enemies, Spawner& spawner);


Vector2 GetShipSpawnPosition(std::vector<std::unique_ptr<Enemy>>& enemies);