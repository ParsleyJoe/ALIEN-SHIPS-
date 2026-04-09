#include "gameAssets.hpp"
#include <chrono>
#include <format>
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
	gLoadSounds(game);

	// Main Enemies vector, now that I think about it maybe its a bad idea to use polymorphism, but
	// I havent have any problems because of it so its being kept
	std::vector<std::unique_ptr<Enemy>> enemies; // NOTE: Polymorphism to store any enemy type
	std::vector<Star> stars;

	// Init Spawners and Holder
	InitSpawners(game.spawnerHolder);

	// Buttons map, and other assets
	UIAssets uiAssets;
	InitUI(uiAssets);

	// For Drawing background stars
	Spawner starSpawner;
	starSpawner.enemyAmmount = 40;
	starSpawner.spawnInterval = 2.0f;

	// Shaders
	RenderTexture2D target = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());
	Shader bloom = LoadShader(0, "resources/shader/bloom_fragment.glsl");

	// GameFont
	GameAssets::gameFont = LoadFont("resources/TitilliumWeb-SemiBold.ttf");
	SetTextureFilter(GameAssets::gameFont.texture, TEXTURE_FILTER_POINT);

	Color backgroundColor = { 13, 13, 13, 255 };
	// Game loop
	while (!WindowShouldClose())
	{
		if (IsKeyPressed(KEY_ESCAPE) && game.currentScene == Scene::GAME)
			game.active = !game.active;

		// Updating Logic ================================
		// -----------------------------------------------
		if (game.active)
		{
			if (game.player.lives <= 0)
			{
				game.active = false;
				game.currentScene = Scene::GAME_OVER;
 
				// All we need for saving, for now
				SaveData sav = gGetSaveData(game);
				sfs::writeEntireFileWithCheckSum( (void*)&sav, sizeof(SaveData), GameAssets::saveFileName);
			}

			UpdateStars(stars, starSpawner);

			UpdatePlayer(game, enemies);
			PowerUpCollision(game.player);

			// Enemy Spawning
			// After this the functions go in a heirarchial order to spawn types of enemies
			StartSpawning(game.wave, enemies, game.spawnerHolder, &game.player);
			
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
		BeginTextureMode(target);

		// Drawing everything first to the renderTexture
		ClearBackground(backgroundColor);
		UIUpdate(uiAssets, game, enemies);
		if (game.currentScene == Scene::MAIN_MENU)
		{
		}
		else if (game.currentScene == Scene::GAME)
		{


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

		}
		else if (game.currentScene == Scene::GAME_OVER)
		{
		}
		DrawUI(uiAssets, game);


		EndTextureMode();

		gDrawingBegin();

		ClearBackground(backgroundColor);

		BeginShaderMode(bloom);

		// Drawing The render Texture on Shaders
		DrawTextureRec(
			target.texture, 
			Rectangle{0, 0, (float)target.texture.width, (float)-target.texture.height},
			{},
			WHITE);

		// To Take Screenshots
		if (IsKeyPressed(KEY_F12))
		{
			auto t = std::chrono::system_clock::now();
			std::string s = std::format("{:%d-%m-%Y %H_%M_%OS}", t);
			std::string filename = "ss" + s + ".png";
			TakeScreenshot(filename.c_str());
		}

		EndShaderMode();


		gDrawingEnd(game);
	}


	rlImGuiShutdown();
	CloseWindow();
	CloseAudioDevice();

	return 0;
}
