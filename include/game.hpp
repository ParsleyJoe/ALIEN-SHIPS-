#pragma once

#include "player.hpp"

struct ImGuiIO;

struct Game {
	ImGuiIO* io;
	Player player;
};

void gInitGame(Game& game);

// InitEverything before drawing
void gDrawingBegin();
void gDrawingEnd(Game& game);

void gLoadTextures(Game& game);
