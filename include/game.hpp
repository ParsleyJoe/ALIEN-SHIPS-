#pragma once

#include "player.hpp"

struct ImGuiIO;

struct Game {
	ImGuiIO* io;
	Player player;
	int wave = 1;
	bool active = false;
	SpawnerHolder spawnerHolder;
};

void gInitGame(Game& game);

// InitEverything before drawing
void gDrawingBegin();
void gDrawingEnd(Game& game);

void gLoadTextures(Game& game);

void gRestartGame(Game &game, std::vector<std::unique_ptr<Enemy>>& enemies);
