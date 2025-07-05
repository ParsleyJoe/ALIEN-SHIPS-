#pragma once
#include <vector>
#include "bulletSpawning.hpp"

namespace GameAssets
{
	inline std::vector<Bullet> enemyBullets;
	inline Texture2D shipSprite;
	inline Texture2D bossHealthBorder;
	inline Texture2D bossSkullSprite;
	constexpr inline Rectangle bullet = { 0, 0, 5, 10 }; // note: x, y, width, height
}