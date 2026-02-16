#pragma once
#include <raylib.h>

// NOTE: Deprecated Component of Project moved everything to UI.hpp, UI.cpp

struct HealthBar
{
	int health;
	Rectangle mainRec;
	Rectangle fillRec;
	Color fillColor;
};

void DrawHealthBar(HealthBar& healthBar, int startHealth);
