#include "stars.hpp"
#include "spawning.hpp"
#include <raylib.h>
#include <gameAssets.hpp>

void UpdateStars(std::vector<Star>& stars, Spawner starSpawner)
{
	MoveStars(stars);
	SpawnStars(stars, starSpawner);
}

void DrawStars(std::vector<Star>& stars)
{

	// Looks better without shaders
	Vector4 shaderValue = ColorNormalize({255, 255, 255});
	int starSize = 2;
	for (Star& star : stars)
	{
		// actual star
		DrawCircle(star.x, star.y, starSize, WHITE);
		// star fake glow
		for (int i = 3; i >= 1; i--)
		{
			Vector2 starPos = { static_cast<float>(star.x), static_cast<float>(star.y) };
			float radius = starSize + i * 1.2f;
			Color glow = Color{ 255, 255, 255, 30 }; // soft white
			DrawCircleV(starPos, radius, glow);
		}
	}
}

void SpawnStars(std::vector<Star>& stars, Spawner spawner)
{
	if (GetTime() - spawner.lastSpawnTime >= spawner.spawnInterval)
	{
		Star star;
		star.x = GetRandomValue(1, GetScreenWidth() - 1);
		star.y = 0;
		stars.push_back(star);
	}
	stars.erase(std::remove_if(stars.begin(), stars.end(), [](const auto& star) {
		return star.y > GetScreenHeight();
		}), stars.end());
}

void MoveStars(std::vector<Star>& stars)
{
	int speed = 8;
	for (Star& star : stars)
	{
		star.y += speed;
	}
}
