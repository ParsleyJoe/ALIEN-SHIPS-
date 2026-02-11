#pragma once
#include <vector>
#include "spawning.hpp"

struct Star
{
	int x;
	int y;
};

void UpdateStars(std::vector<Star>& stars, Spawner spawner);
void DrawStars(std::vector<Star>& stars);
void SpawnStars(std::vector<Star>& stars, Spawner spawner);
void MoveStars(std::vector<Star>& stars);
