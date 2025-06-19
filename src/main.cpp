#include "raylib.h"
#include <raymath.h>
#include <iostream>
#include <algorithm>
#pragma region imgui
#include "imgui.h"
#include "rlImGui.h"
#include "imguiThemes.h"
#pragma endregion

#include <vector>

#include "enemy.hpp"
#include "spawning.hpp"
#include "player.hpp"
#include "bulletSpawning.hpp"
#include "sceneManager.hpp"
#include "button.hpp"

bool gameActive = false;

//----------------------------------------------------------------------------------
int main()
{
	// Raylib Init
	//SetConfigFlags();
	InitWindow(800, 600, "SpaceGame");
	SetTargetFPS(60);

#pragma region imgui
	rlImGuiSetup(true);

	//you can use whatever imgui theme you like!
	//ImGui::StyleColorsDark();
	//imguiThemes::yellow();
	//imguiThemes::gray();
	//imguiThemes::green();
	//imguiThemes::red();
	imguiThemes::embraceTheDarkness();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.FontGlobalScale = 2;

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		//style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 0.5f;
		//style.Colors[ImGuiCol_DockingEmptyBg].w = 0.f;
	}

#pragma endregion

	//TODO: Might move it to GameAssets namespace
	// Game "Assets"
	Rectangle bullet = { 0, 0, 5, 10 }; // note: x, y, width, height
	Rectangle enemy = { 400, 50, 25, 25 }; // "Template" enemy rectangle
	std::vector<std::unique_ptr<Enemy>> enemies; // !Polymorphism to store any enemy type
	std::vector<BulletSpawner> bltSpawners;

	Texture2D playerSprite = LoadTexture("resources/ship.png");
	GameAssets::shipSprite = LoadTexture("resources/UFO.png");

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
	circleShooter.waveType = EnemyType::CIRCLESHOOTER;
	circleShooter.enemyAmmount = 1;
	circleShooter.spawnInterval = 2.0f;

	Spawner asteroidSpawner;
	asteroidSpawner.waveType = EnemyType::ASTEROID;
	asteroidSpawner.spawnInterval = GetRandomValue(15, 25) + 0.0f;
	asteroidSpawner.enemyAmmount = 1;

	Button btn;
	btn.rec = Rectangle{ (GetScreenWidth() / 2) - 50.0f, (GetScreenHeight() / 2) - 0.0f, 120, 50 };
	btn.color = DARKGRAY;
	btn.text = "Start Game";
	btn.txtColor = WHITE;
	btn.fontSize = 15;

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

			for (const auto& enemy : enemies)
			{
				enemy->Update();
			}

			BulletsHit(player.playerBullets, enemies);

			ShootBullet(player, bullet, player.playerBullets);

			for (Bullet& blt : GameAssets::enemyBullets)
			{
				blt.Update();
			}
		}

		BeginDrawing();
		ClearBackground(RAYWHITE);

#pragma region imgui
		rlImGuiBegin();

		ImGui::PushStyleColor(ImGuiCol_WindowBg, {});
		ImGui::PushStyleColor(ImGuiCol_DockingEmptyBg, {});
		ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
		ImGui::PopStyleColor(2);
#pragma endregion

		ImGui::Begin("Test");
		ImGui::Text("Size of bullets Vector: %i", GameAssets::enemyBullets.size());
		ImGui::End();

		if (currentScene == Scene::MAIN_MENU)
		{
			DrawText("SPACE BULLET HELL!!", (GetScreenWidth() / 2) - 170, (GetScreenHeight() / 2) - 50, 30, DARKGRAY);
			DrawButton(btn);
			if (IsButtonClicked(btn))
			{
				gameActive = true;
				currentScene = Scene::GAME;
			}
		}
		else if (currentScene == Scene::GAME)
		{
			DrawText(TextFormat("%d", player.lives), 10, 10, 20, BLACK);

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
#pragma region imgui
		rlImGuiEnd();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}
#pragma endregion

		EndDrawing();
	}

#pragma region imgui
	rlImGuiShutdown();
#pragma endregion

	UnloadTexture(playerSprite);
	CloseWindow();

	return 0;
}