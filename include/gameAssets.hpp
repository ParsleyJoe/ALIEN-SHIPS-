#pragma once
#include <map>
#include <string>
#include <vector>
#include "bulletSpawning.hpp"
#include <powerup.hpp>

#ifndef DEBUG
#define DEBUG 1
#endif

// Global Variables
// ----------------
namespace GameAssets
{
	// vector with bullet of every enemy
	inline std::vector<Bullet> enemyBullets;

	inline std::vector<PowerUp> powerUps;

	// Sprites
	inline Texture2D shipSprite;
	inline Texture2D enemyBulletSprite;
	inline Texture2D bossHealthBorder;
	inline Texture2D bossSkullSprite;
	inline Texture2D fodderSprite;
	inline Texture2D sinSprite;
	inline Texture2D asteroidSprite;
	inline Texture2D creeperSprite;

	inline std::map<std::string, Texture2D> powerUpSprites; // Speed, Power, OneUp, Shield
	constexpr inline Rectangle bullet = { 0, 0, 5, 10 }; // note: x, y, width, height
	
	inline const char* saveFileName = "saveData.sav";

	inline float gameStartTime = 0.0f;

	inline Font gameFont;
	inline Sound enemyHitSound;
	inline Sound enemyDieSound;
}
