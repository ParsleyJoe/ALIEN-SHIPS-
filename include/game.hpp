#pragma once

#include "player.hpp"
#include "sceneManager.hpp"
#include <memory>

struct ImGuiIO;

struct Game {
	ImGuiIO* io;
	Player player;
	int wave = 1;
	bool active = false;
	SpawnerHolder spawnerHolder;
	Scene currentScene = Scene::MAIN_MENU;

	int score = 0;
};

struct SaveData {
	int score = 0;
};

void gInitGame(Game& game);

// InitEverything before drawing
void gDrawingBegin();
void gDrawingEnd(Game& game);

void gLoadTextures(Game& game);
void gLoadSounds(Game& game);

void gRestartGame(Game& game, std::vector<std::unique_ptr<Enemy>>& enemies);

// Called when player kills an enemy
void gEnemyKilled(Game& game, std::unique_ptr<Enemy>& enemy);

// Returns updated data based on game struct
SaveData gGetSaveData(Game& game);
