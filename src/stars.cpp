#include "stars.hpp"
#include <raylib.h>

void DrawStars(std::vector<Star>& stars)
{
	int starSize = 1;
	for (Star& star : stars)
	{
		// actual star
		DrawRectangle(star.x, star.y, 1, 1, WHITE);
		// star fake glow
		for (int i = 3; i >= 1; i--)
		{
			Vector2 starPos = { star.x, star.y };
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