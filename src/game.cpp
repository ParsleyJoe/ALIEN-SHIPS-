#include "gameAssets.hpp"
#include "player.hpp"
#include "raylib.h"
#include "safeSave.h"
#include "spawning.hpp"
#include <iostream>
#include <memory>
#include <rlImGui.h>
#include <imguiThemes.h>
#include <game.hpp>

void gLoadTextures(Game& game)
{
	pLoadTxt(game.player);

	// Player Sprite
	GameAssets::shipSprite = LoadTexture("resources/UFO.png");

	// Boss Sprites
	GameAssets::bossHealthBorder = LoadTexture("resources/bossHealthBorder.png");
	GameAssets::bossSkullSprite = LoadTexture("resources/skull.png");

	GameAssets::fodderSprite = LoadTexture("resources/fodder.png");
	GameAssets::asteroidSprite = LoadTexture("resources/assteroid.png");
	GameAssets::sinSprite = LoadTexture("resources/sinShooter.png");
	GameAssets::enemyBulletSprite = LoadTexture("resources/enemyBullet.png");
	GameAssets::creeperSprite = LoadTexture("resources/creeper.png");

	GameAssets::powerUpSprites["Speed"] = LoadTexture("resources/speed.png");
	GameAssets::powerUpSprites["Power"] = LoadTexture("resources/power.png");
	GameAssets::powerUpSprites["OneUp"] = LoadTexture("resources/gear.png");
	GameAssets::powerUpSprites["Shield"] = LoadTexture("resources/shield.png");
}

void gLoadSounds(Game& game)
{
	pLoadSounds(game.player);
}


void gInitGame(Game& game)
{
	InitWindow(800, 600, "SpaceGame");
	SetTargetFPS(60);
	SetExitKey(KEY_F4);
	InitAudioDevice();

	rlImGuiSetup(true);


	imguiThemes::embraceTheDarkness();

	game.io = &ImGui::GetIO(); (void)game.io;
	game.io->ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	game.io->ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	game.io->FontGlobalScale = 2;

	ImGuiStyle& style = ImGui::GetStyle();
	if (game.io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}

	GameAssets::gameStartTime = GetTime();
}

void gRestartGame(Game &game, std::vector<std::unique_ptr<Enemy>>& enemies)
{
	PlayerRestart(game.player);
	game.player.specialMeter = 0;

	game.player.lives = 3;
	game.wave = 1;
	game.active = true;
	game.score = 0;
	game.currentScene = Scene::GAME;

	enemies.clear();

	GameAssets::enemyBullets.clear();
	GameAssets::powerUps.clear();

	GameAssets::gameStartTime = GetTime();

	game.player.playerBullets.clear();
	InitSpawners(game.spawnerHolder);
}

void gEnemyKilled(Game &game, std::unique_ptr<Enemy>& enemy)
{
	game.score += enemy->scoreIncrement;
	std::cout << "Score is now: " << game.score << std::endl;
}

void gDrawingBegin()
{
	BeginDrawing();

	rlImGuiBegin();

	ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
	ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
	ImGui::PopStyleColor(2);

	ImGui::Begin("Test");
	ImGui::Text("FPS: %i", GetFPS());
	ImGui::Text("Size of bullets Vector: %zu", GameAssets::enemyBullets.size());
//	ImGui::Text("Star vector size: %zu", stars.size());

}

void gDrawingEnd(Game& game)
{
	ImGui::End();
	rlImGuiEnd();

	if (game.io->ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	EndDrawing();

}

SaveData gGetSaveData(Game& game)
{
	void* rawData = new SaveData;
	sfs::readEntireFileWithCheckSum(rawData, sizeof(SaveData),  GameAssets::saveFileName);
	SaveData* saveData = static_cast<SaveData*>(rawData);

	if (game.score > saveData->score)
		saveData->score = game.score;
	std::cout << "HighScore: " << saveData->score << std::endl;
	return (SaveData)*saveData;
}
