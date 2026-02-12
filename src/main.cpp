#include "raylib.h"
#include <raymath.h>
#include "rlImGui.h"

#include <vector>

#include <game.hpp>
#include "enemy.hpp"
#include "spawning.hpp"
#include "player.hpp"
#include "bulletSpawning.hpp"
#include "sceneManager.hpp"
#include "button.hpp"
#include "stars.hpp"


//----------------------------------------------------------------------------------
int main()
{
	Game game;

	// Initialize Raylib and Load game textures
	gInitGame(game);
	gLoadTextures(game);

	Shader bloom = LoadShader(0, "resources/shader/bloom_fragment.glsl");

	std::vector<std::unique_ptr<Enemy>> enemies; // NOTE: Polymorphism to store any enemy type
	std::vector<Star> stars;

	int wave = 1;
	int lives = 3;
	
	bool gameActive = false;
	Scene currentScene = Scene::MAIN_MENU;

	// Init Spawners and Holder
	SpawnerHolder spawnerHolder;
	InitSpawners(spawnerHolder);

	// For Drawing background stars
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
		if (IsKeyPressed(KEY_P) && currentScene == Scene::GAME)
			gameActive = !gameActive;


		// Updating Logic ================================
		// -----------------------------------------------
		if (gameActive)
		{
			if (game.player.lives <= 0)
			{
				gameActive = false;
				currentScene = Scene::GAME_OVER;
			}

			UpdateStars(stars, starSpawner);

			UpdatePlayer(game.player, enemies);

			// Enemy Spawning
			StartSpawning(wave, enemies, spawnerHolder);
			
			EnemyUpdateContext context = { enemies };
			for (int i = 0; i < context.enemies.size(); i++)
			{
				enemies[i]->Update(context);
			}


			for (Bullet& blt : GameAssets::enemyBullets)
			{
				blt.Update();
			}
			for (Bullet& blt : game.player.playerBullets)
			{
				blt.Update();
			}
		}

		// Drawing =======================================
		// -----------------------------------------------
		gDrawingBegin();

		//BeginShaderMode(bloom);

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

			DrawText(TextFormat("%d", game.player.lives), 10, 10, 20, RAYWHITE);

			DrawStars(stars);
			// Player and enemies Drawing
			DrawPlayer(game.player);
			for (const auto& enemy : enemies)
			{
				enemy->Draw();
			}

			// Draw Bullets
			for (Bullet& bullet : game.player.playerBullets)
			{
				bullet.Draw();
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

		//EndShaderMode();
		gDrawingEnd(game);
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}
