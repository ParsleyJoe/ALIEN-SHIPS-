#include "gameAssets.hpp"
#include <raylib.h>
#include <raymath.h>
#include <rlImGui.h>

#include <vector>

#include "game.hpp"
#include "enemy.hpp"
#include "spawning.hpp"
#include "player.hpp"
#include "bulletSpawning.hpp"
#include "sceneManager.hpp"
#include "button.hpp"
#include "stars.hpp"
#include "powerup.hpp"
#include "UI.hpp"

#include <safeSave.h>

//----------------------------------------------------------------------------------
int main()
{
	Game game;

	// Initialize Raylib and Load game textures
	gInitGame(game);
	gLoadTextures(game);

	std::vector<std::unique_ptr<Enemy>> enemies; // NOTE: Polymorphism to store any enemy type
	std::vector<Star> stars;

	Scene currentScene = Scene::MAIN_MENU;

	// Init Spawners and Holder
	InitSpawners(game.spawnerHolder);

	UIAssets uiAssets;
	InitUI(uiAssets);

	// For Drawing background stars
	Spawner starSpawner;
	starSpawner.enemyAmmount = 40;
	starSpawner.spawnInterval = 2.0f;


	//! Game loop
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_ESCAPE) && currentScene == Scene::GAME)
			game.active = !game.active;

		// Updating Logic ================================
		// -----------------------------------------------
		if (game.active)
		{
			if (game.player.lives <= 0)
			{
				game.active = false;
				currentScene = Scene::GAME_OVER;

				SaveData sav = gGetSaveData(game);
				sfs::writeEntireFileWithCheckSum( (void*)&sav, sizeof(SaveData), GameAssets::saveFileName);
			}

			UpdateStars(stars, starSpawner);

			UpdatePlayer(game, enemies);
			PowerUpCollision(game.player);

			// Enemy Spawning
			StartSpawning(game.wave, enemies, game.spawnerHolder);
			
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

		DrawUI(uiAssets, currentScene, game);
		if (currentScene == Scene::MAIN_MENU)
		{
			if (IsButtonClicked(uiAssets.btns["Start"]))
			{
				game.active = true;
				currentScene = Scene::GAME;
				gRestartGame(game, enemies);
			}
		}
		else if (currentScene == Scene::GAME)
		{

			DrawText(TextFormat("%d", game.player.lives), 10, 10, 20, RAYWHITE);

			DrawStars(stars);
			DrawPowerUps();

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
				blt.Draw();
			}
			if (!game.active)
			{
				DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GRAY, 0.5f));
				DrawText("Paused", 600, 300, 40, GRAY);
			}
		}
		else if (currentScene == Scene::GAME_OVER)
		{
			if (IsButtonClicked(uiAssets.btns["Restart"]))
			{
				gRestartGame(game, enemies);
				currentScene = Scene::GAME;
				
			}
		}

		DrawText(TextFormat("Game Time: %.2f", GetTime() - GameAssets::gameStartTime), 20, 20, 20, WHITE);

		gDrawingEnd(game);
	}

	rlImGuiShutdown();

	CloseWindow();

	return 0;
}
