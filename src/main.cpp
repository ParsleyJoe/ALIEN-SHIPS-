#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include <raymath.h>
#include <iostream>
#include <algorithm>
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"

#include <vector>

#include "enemy.hpp"
#include "spawning.hpp"
#include "player.hpp"
#include "bulletSpawning.hpp"
#include "sceneManager.hpp"
#include "button.hpp"
#include "stars.hpp"

bool gameActive = false;

//----------------------------------------------------------------------------------
int main()
{
	// Raylib Init
	//SetConfigFlags();
	InitWindow(800, 600, "SpaceGame");
	SetTargetFPS(60);

	rlImGuiSetup(true);

	imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
	}

	// TODO: Might move it to GameAssets namespace
	// Game "Assets"
	std::vector<std::unique_ptr<Enemy>> enemies; // !Polymorphism to store any enemy type
	std::vector<Star> stars;

	Texture2D playerSprite = LoadTexture("resources/ship.png");
	GameAssets::shipSprite = LoadTexture("resources/UFO.png");
	GameAssets::bossHealthBorder = LoadTexture("resources/bossHealthBorder.png");
	GameAssets::bossSkullSprite = LoadTexture("resources/skull.png");

	Player player;
	int wave = 1;
	int lives = 3;

	Scene currentScene = Scene::MAIN_MENU;

	Spawner fodderSpawner;
	fodderSpawner.enemyAmmount = 10;
	fodderSpawner.waveType = EnemyType::FODDER;
	fodderSpawner.spawnInterval = 0.6f;

	Spawner shipSpawner;
	shipSpawner.waveType = EnemyType::SHIP;
	shipSpawner.spawnInterval = 0.3f;
	shipSpawner.enemyAmmount = 15;

	Spawner circleShooter;
	circleShooter.waveType = EnemyType::SINWAVE_SHOOTER;
	circleShooter.enemyAmmount = 1;
	circleShooter.spawnInterval = 2.0f;

	Spawner bossSpawner;
	bossSpawner.waveType = EnemyType::BOSS_SHIP;
	bossSpawner.enemyAmmount = 1;
	bossSpawner.spawnInterval = 4.0f;

	Spawner asteroidSpawner;
	asteroidSpawner.waveType = EnemyType::ASTEROID;
	asteroidSpawner.spawnInterval = GetRandomValue(15, 25) + 0.0f;
	asteroidSpawner.enemyAmmount = 1;

	Spawner starSpawner;
	starSpawner.enemyAmmount = 40;
	starSpawner.spawnInterval = 2.0f;

	Button startBtn;
	startBtn.rec = Rectangle{ (GetScreenWidth() / 2) - 50.0f, (GetScreenHeight() / 2) - 0.0f, 120, 50 };
	startBtn.color = DARKGRAY;
	startBtn.text = "Start Game";
	startBtn.txtColor = WHITE;
	startBtn.fontSize = 15;

	//! Game loop
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_P))
			gameActive = !gameActive;

		// Update Logic, Don't update anything if paused
		if (gameActive)
		{
			if (player.lives <= 0)
			{
				gameActive = false;
				currentScene = Scene::GAME_OVER;
			}

			MoveStars(stars);
			SpawnStars(stars, starSpawner);

			MovePlayer(player);
			PlayerCollision(player, enemies);

			switch (wave)
			{
			case 1:
				SpawnEnemies(wave, enemies, fodderSpawner);
				break;
			case 2:
				SpawnEnemies(wave, enemies, shipSpawner);
				break;
			case 3:
				SpawnEnemies(wave, enemies, circleShooter);
				break;
			}
			SpawnAsteroid(enemies, asteroidSpawner);

			EnemyUpdateContext context = { enemies };
			for (int i = 0; i < context.enemies.size(); i++)
			{
				enemies[i]->Update(context);
			}

			BulletsHit(player.playerBullets, enemies);

			ShootBullet(player, GameAssets::bullet, player.playerBullets);

			for (Bullet& blt : GameAssets::enemyBullets)
			{
				blt.Update();
			}
		}

		BeginDrawing();
		ClearBackground(Color{ 13, 13, 13, 255 });

		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);

		ImGui::Begin("Test");
		ImGui::Text("FPS: %i", GetFPS());
		ImGui::Text("Size of bullets Vector: %i", player.playerBullets.size());
		ImGui::Text("Star vector size: %i", stars.size());
		ImGui::End();

		if (currentScene == Scene::MAIN_MENU)
		{
			DrawText("SPACE BULLET HELL!!", (GetScreenWidth() / 2) - 170, (GetScreenHeight() / 2) - 50, 30, DARKGRAY);
			DrawButton(startBtn);
			if (IsButtonClicked(startBtn))
			{
				gameActive = true;
				currentScene = Scene::GAME;
			}
		}
		else if (currentScene == Scene::GAME)
		{
			DrawText(TextFormat("%d", player.lives), 10, 10, 20, RAYWHITE);

			DrawStars(stars);
			// Player and enemies Drawing
			DrawPlayer(player, playerSprite);
			for (const auto& enemy : enemies)
			{
				enemy->Draw();
			}

			// Draw Bullets
			for (Rectangle& bullet : player.playerBullets)
			{
				DrawRectangleRec(bullet, RED);
			}
			for (auto& blt : GameAssets::enemyBullets)
			{
				DrawRectangleRec(blt.rec, RED);
			}
			if (!gameActive)
			{
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GRAY, 0.5f));
				DrawText("Paused", 600, 300, 40, GRAY);
			}
		}
		else if (currentScene == Scene::GAME_OVER)
		{
			int textWidth = MeasureText("GAME OVER", 30);
			int x = (GetScreenWidth() / 2) - (textWidth / 2);
			int y = (GetScreenHeight() / 2) - (30 / 2); // optional, for vertical center
			DrawText("GAME OVER", x, y, 30, DARKGRAY);
		}
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		EndDrawing();
	}

	rlImGuiShutdown();

	UnloadTexture(playerSprite);
	CloseWindow();

	return 0;
}
