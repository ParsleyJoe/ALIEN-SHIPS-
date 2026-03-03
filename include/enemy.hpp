#pragma once
#include "raylib.h"
#include <vector>
#include <memory>
#include "bulletSpawning.hpp"
#include "hud.hpp"
#include "spawning.hpp"

enum class EnemyType {
	FODDER, SHIP, SINWAVE_SHOOTER, ASTEROID, BOSS_SHIP, RADIATING_SHOOTER, BARRIER
};

struct EnemyUpdateContext 
{
	std::vector<std::unique_ptr<Enemy>>& enemies;
};
// Using inheritance for different enemies
struct Enemy
{
	EnemyType type = EnemyType::FODDER;
	Rectangle rec = { 300, 100, 25, 25 };
	int health = 30;

	bool alive = true;

	int scoreIncrement = 5;

	virtual void Draw() = 0;
	virtual void Update(EnemyUpdateContext& ctx) = 0;
	void Die();
};

struct Fodder : Enemy
{
	int speedX = 5;
	int speedY = 2;
	float sizeDivider = 1.3f;
	void Draw() override;
	void Update(EnemyUpdateContext& ctx) override;
	Fodder();
};

struct Ship : Enemy
{
	BulletSpawner bltSpawner;
	int speed = 3;
	void Draw() override;
	void Update(EnemyUpdateContext& ctx) override;
	Ship();
};

struct SinwaveShooter : Enemy
{
	BulletSpawner bltSpawner;
	float shootAngle;
	float sinMultiplier = 0.0f;
	void Update(EnemyUpdateContext& ctx) override;
	void Draw() override;
	SinwaveShooter();
};

struct RadialShooter : Enemy
{
	int speed = 3;
	float angle = 36;
	BulletSpawner bltSpawner;
	int cooldown = 3.0f;
	void Update(EnemyUpdateContext& ctx) override;
	void Draw() override;
	void Shoot();
	RadialShooter();
};

struct BossShip : Enemy
{
	HealthBar healthBar;
	int startHealth;
	Spawner radialShooterSpawner;
	BossShip();
	void Update(EnemyUpdateContext& ctx) override;
	void Draw() override;
};

struct Asteroid : Enemy
{
	int speedX = 15;
	int speedY = 15;
	float sizeDivider = 1.4f;

	void Draw() override;
	void Update(EnemyUpdateContext& ctx) override;
	Asteroid();
};

struct Barrier : Enemy
{
	float activeTime = 4.0f; // Should be active for 
	float activeFor = 0.0f;  // Is active for

	void Draw() override;
	void Update(EnemyUpdateContext& ctx) override;
	Barrier();
};
