#pragma once
#include <vector>
#include "bulletSpawning.hpp"
#include <powerup.hpp>

namespace GameAssets
{
	// vector with bullet of every enemy
	inline std::vector<Bullet> enemyBullets;
	inline std::vector<PowerUp> powerUps;

	// Sprites
	inline Texture2D shipSprite;
	inline Texture2D bossHealthBorder;
	inline Texture2D bossSkullSprite;
	inline Texture2D fodderSprite;
	inline Texture2D sinSprite;
	inline Texture2D asteroidSprite;
	constexpr inline Rectangle bullet = { 0, 0, 5, 10 }; // note: x, y, width, height
}
