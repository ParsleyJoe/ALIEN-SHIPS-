#include "enemy.hpp"
#include "bulletSpawning.hpp"
#include "player.hpp"
#include "raylib.h"
#include <iostream>
#include <random>
#include "powerup.hpp"
#include "gameAssets.hpp"
#include "raymath.h"


// NOTE: Base Enemy Class Functions
// --------------------------
void Enemy::Die()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> chance(0,4);

	if (chance(gen) == 0)
		SpawnPowerUp({rec.x, rec.y});
	PlaySound(GameAssets::enemyDieSound);
	alive = false;
}

Enemy::Enemy()
{
}

// NOTE: Fodder Functions
// ----------------------
void Fodder::Draw()
{
	if (IsTextureReady(GameAssets::fodderSprite))
		DrawTexturePro(GameAssets::fodderSprite, {0, 0, (float)GameAssets::fodderSprite.width, (float)GameAssets::fodderSprite.height}
	  , rec, {0}, 0.0f, WHITE);
	else
		DrawCircle(rec.x + (rec.width / 2), rec.y + (rec.height / 2), rec.width / sizeDivider, RED);

	if (DEBUG)
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, DARKGREEN);
}
void Fodder::Update(EnemyUpdateContext& ctx)
{
	int rightBound = GetScreenWidth() - rec.width;
	if ((rec.x) >= rightBound)
	{
		rec.x = rightBound;
		speedX = -speedX;
	}
	else if (rec.x <= 0)
	{
		rec.x = 0;
		speedX = -speedX;
	}

	// Move
	rec.x += speedX;
	rec.y += speedY;
}

Fodder::Fodder()
{
	type = EnemyType::FODDER;
	rec = {0, 0, 60, 60};
}


// NOTE: Ship Functions
// --------------------
void Ship::Draw()
{
	if (DEBUG)
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, BLACK);
	DrawTexture(GameAssets::shipSprite, rec.x, rec.y, WHITE);
}

void Ship::Update(EnemyUpdateContext& ctx)
{
	rec.y += speed; // moving only on y
	bltSpawner.position.y += speed;
	SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, DEG2RAD * 0.0f);
}
Ship::Ship()
{
	type = EnemyType::SHIP;
}

// NOTE: SinWaveShooter Functions
// ------------------------------
void SinwaveShooter::Update(EnemyUpdateContext& ctx)
{
	// TODO: Move it, Haven't decided how I'll Move it....
	sinMultiplier = sinf(GetTime() * 10.0f); // amplify the curve speed by 10.0f
	SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, shootAngle * sinMultiplier);
}

void SinwaveShooter::Draw()
{
	if (IsTextureReady(GameAssets::sinSprite))
		DrawTexturePro(GameAssets::sinSprite, {0, 0, static_cast<float>(GameAssets::sinSprite.width), static_cast<float>(GameAssets::sinSprite.height)},
	  rec, {rec.width, rec.height}, 180.0f, WHITE);
	else
		DrawRectangleRec(rec, BLUE);
	if (DEBUG)
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, WHITE);
}
SinwaveShooter::SinwaveShooter()
{
	rec = {0, 0, 40, 40};
	type = EnemyType::SINWAVE_SHOOTER;
}


// NOTE: RadialShooter Functions
// -----------------------------
void RadialShooter::Update(EnemyUpdateContext& ctx)
{
	float timeUntilShoot = (bltSpawner.lastSpawned + cooldown) - GetTime();
	float timeSinceShot = (GetTime() - bltSpawner.lastSpawned);
	if (!(timeUntilShoot <= 1.0f) && (timeSinceShot >= 1.0f)) // If not about to shoot move
	{
		rec.y += speed;
	}

	if (GetTime() - bltSpawner.lastSpawned >= cooldown)
	{
		Shoot();
	}
}

void RadialShooter::Draw()
{
	DrawRectangleRec(rec, RED);
}

void RadialShooter::Shoot()
{
	for (int i = 0; i < 10; i++)
	{
		bltSpawner.position = Vector2{ rec.x, rec.y };
		SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, angle);
		bltSpawner.direction = Vector2Rotate(bltSpawner.direction, angle);
	}
	bltSpawner.lastSpawned = GetTime();
}

RadialShooter::RadialShooter()
{
	type = EnemyType::RADIATING_SHOOTER;
}

// NOTE: BossShip Functions
// ------------------------
BossShip::BossShip()
{
	startHealth = 1500;
	health = startHealth;
	healthBar.mainRec = Rectangle{ GetScreenWidth() - 50.0f, 40.0f, 30.0f, GetScreenHeight() - 170.0f };
	healthBar.fillRec = Rectangle{ healthBar.mainRec.x + 3, healthBar.mainRec.y + 3, healthBar.mainRec.width - 6, healthBar.mainRec.height - 6 };

	radialShooterSpawner.waveType = EnemyType::RADIATING_SHOOTER;
	radialShooterSpawner.spawnInterval = 5.0f;

	type = EnemyType::BOSS_SHIP;
}

void BossShip::Update(EnemyUpdateContext& ctx)
{
	healthBar.health = health;

	if (GetTime() - radialShooterSpawner.lastSpawnTime >= radialShooterSpawner.spawnInterval)
	{
		SpawnRadialShooter(ctx.enemies, radialShooterSpawner, rec);
		radialShooterSpawner.lastSpawnTime = GetTime();
	}
}

void BossShip::Draw()
{
	// Drawing the boss
	DrawRectangle(rec.x, rec.y, rec.width, rec.height, GREEN);

	// Drawing health bar and skull sprite
	DrawHealthBar(healthBar, startHealth);
	DrawTexture(GameAssets::bossSkullSprite, healthBar.mainRec.x, healthBar.mainRec.y + healthBar.mainRec.height + 10, WHITE);
}


// NOTE:Asteroid Functions
// -----------------------
void Asteroid::Draw()
{
	if (IsTextureReady(GameAssets::asteroidSprite))
	{
		DrawTexturePro(GameAssets::asteroidSprite, {0, 0, (float)GameAssets::asteroidSprite.width, (float)GameAssets::asteroidSprite.height},
	  rec, {0}, 0.0f, WHITE);
	}
	else
		DrawCircle(rec.x + (rec.width / 2), rec.y + (rec.height / 2), rec.width / sizeDivider, RED);
	
	if (DEBUG)
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, GREEN);
}

void Asteroid::Update(EnemyUpdateContext& ctx)
{
	rec.x += speedX;
	rec.y += speedY;
}

Asteroid::Asteroid()
{
	type = EnemyType::ASTEROID;
	health = 999;
}


// NOTE: Barrier Functions
// -----------------------
Barrier::Barrier()
{
	type = EnemyType::BARRIER;
	health = 300;

	int t = GetRandomValue(0, 2); // Type of barrier
	t = 0;
	switch (t) 
	{
	case 0:
		rec = {150.0f, GetScreenHeight() - 50.0f, 30.0f, 30.0f};
		break;
	case 1:
		break;
	case 2:
		break;
	default:
		std::cout << "Barrier Constructor Undefined t var" << '\n';
	}

	bltSpawner = MakeSpawner({rec.x, rec.y}, 0.0f);
	bltSpawner.direction = {0, -1};
}

void Barrier::Draw()
{
	if (IsTextureReady(GameAssets::barrierSprite))
	{
		DrawTexturePro(GameAssets::barrierSprite, {0.0f, 0.0f, (float)GameAssets::barrierSprite.width, (float)GameAssets::barrierSprite.height},
		 rec, {}, 0.0f, WHITE);
	}
	else
		DrawRectangleRec(rec, Fade(GREEN, 0.7f));
	if (DEBUG)
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, RED);

}

void Barrier::Update(EnemyUpdateContext& ctx)
{
	aliveFor += GetFrameTime();
	
	if (aliveFor > startMovingIn)
		rec.y += speed;

	Vector2 center = {rec.x + (rec.width * 0.5f), rec.y + (rec.height * 0.5f)};
	bltSpawner.position = center;
	SpawnBullets(bltSpawner, GameAssets::enemyBullets, GameAssets::bullet, DEG2RAD * 0.0f);
}

// NOTE: Barrier Functions
// -----------------------
Creeper::Creeper()
{
	health = 40;
	type = EnemyType::CREEPER;
	rec = {0.0f, 0.0f, 30.0f, 30.0f};
}

void Creeper::Draw()
{
	if (!alive) return;
	if (DEBUG)
	{
		DrawRectangleLines(rec.x, rec.y, rec.width, rec.height, RED);
	}

	if (exploding)
	{
		if (DEBUG)
			DrawCircleLines(rec.x + (rec.width * 0.5f), rec.y + (rec.height * 0.5f), explosionRadius, RED);
		
		float offset = (rec.width) * 0.5f;
		float whiteSpaceAdjust = 7.0f;
		Rectangle destRec = {rec.x - offset - whiteSpaceAdjust, rec.y - rec.height * 0.5f - whiteSpaceAdjust, rec.width + explosionRadius + whiteSpaceAdjust, rec.height + explosionRadius + whiteSpaceAdjust};
		DrawTexturePro(GameAssets::explosionSprite, {0.0f, 0.0f, (float)GameAssets::explosionSprite.width, (float)GameAssets::explosionSprite.width},
		 destRec, {}, 0.0f, WHITE);
	}
	else
	{

		DrawTexturePro(GameAssets::creeperSprite,{0.0f, 0.0f, (float)GameAssets::creeperSprite.width, (float)GameAssets::creeperSprite.height},
		 rec, {}, 0.0f, WHITE);
	}
}

void Creeper::Update(EnemyUpdateContext& ctx)
{
	if (!alive) return;
	Vector2 playerPos = {
		player->rec.x + player->rec.width / 2.0f,
		player->rec.y + player->rec.height / 2.0f
	};
	Vector2 pos = {
        rec.x + rec.width / 2.0f,
        rec.y + rec.height / 2.0f
	};
	float dist = Vector2Distance(pos, playerPos);

	// Explodde before reaching player to give a better effect
	float explodeOffset = 20.0f;
	if (exploding || dist <= explosionRadius + 20.0f)
	{
		Explode();
	}
	else
	{
		Move(playerPos, pos);
	}
}

void Creeper::Move(Vector2& playerPos, Vector2& pos)
{
	Vector2 dir = Vector2Normalize(Vector2Subtract(playerPos, pos));
	rec.x += dir.x * speed;
	rec.y += dir.y * speed; 
}

void Creeper::Explode()
{
	if (!exploding) exploding = true;

	time += GetFrameTime();
	if (time >= explodeFor)
	{
		Die();
		time = 0.0f;
	}
}
