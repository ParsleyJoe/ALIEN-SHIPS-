#pragma once
#include <vector>
#include "bulletSpawning.hpp"

namespace GameAssets
{
	inline std::vector<Bullet> enemyBullets;
	inline Texture2D shipSprite;
	constexpr inline Rectangle bullet = { 0, 0, 5, 10 }; // note: x, y, width, height
}