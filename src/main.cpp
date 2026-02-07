#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include "raylib.h"
#include <raymath.h>
#include "imgui.h"
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

bool gameActive = false;

//----------------------------------------------------------------------------------
int main()
{
	// Raylib Init
	//SetConfigFlags();
	Game game;
	InitGame(game);

	// TODO: Might move it to GameAssets namespace
	// Game "Assets"
	std::vector<std::unique_ptr<Enemy>> enemies; // !Polymorphism to store any enemy type
	std::vector<Star> stars;

	Texture2D playerSprite = LoadTexture("resources/ship.png");
	GameAssets::shipSprite = LoadTexture("resources/UFO.png");
	GameAssets::bossHealthBorder = LoadTexture("resources/bossHealthBorder.png");
	GameAssets::bossSkullSprite = LoadTexture("resources/skull.png");
	GameAssets::starShader = LoadShader("resources/shader/star_vertex.glsl", "resources/shader/star_fragment.glsl");

	Player player;
	int wave = 1;
	int lives = 3;

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
		if (IsKeyPressed(KEY_P))
			gameActive = !gameActive;

		// Updating Logic ================================
		// -----------------------------------------------
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

			StartSpawning(wave, enemies, spawnerHolder);
			
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

		// Drawing =======================================
		// -----------------------------------------------
		gDrawingBegin();

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
		
		gDrawingEnd(game);
	}

	rlImGuiShutdown();

	UnloadTexture(playerSprite);
	CloseWindow();

	return 0;
}
