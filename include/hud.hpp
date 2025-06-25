#pragma once
#include <raylib.h>

struct HealthBar
{
	int health;
	Rectangle mainRec;
	Rectangle fillRec;
	Color fillColor;
};

void DrawHealthBar(HealthBar& healthBar, int startHealth);