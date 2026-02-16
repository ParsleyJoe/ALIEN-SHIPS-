#include "button.hpp"
#include "gameAssets.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "raylib.h"
#include <UI.hpp>
#include <sceneManager.hpp>

void DrawUI(UIAssets& assets, Scene& currentScene, Player& player)
{
	switch (currentScene)
	{
	case Scene::MAIN_MENU:
		DrawText("SPACE BULLET HELL!!", (GetScreenWidth() / 2) - 170, (GetScreenHeight() / 2) - 50, 30, DARKGRAY);
		DrawButton(assets.btns["Start"]);
		break;
	case Scene::GAME:
		DrawEffectTimers(player);
		break;
	case Scene::GAME_OVER:
		int textWidth = MeasureText("GAME OVER", 30);
		int x = (GetScreenWidth() / 2) - (textWidth / 2);
		int y = (GetScreenHeight() / 2) - (30 / 2); // optional, for vertical center
		DrawText("GAME OVER", x, y, 30, DARKGRAY);

		Button btn = assets.btns["Restart"];
		DrawButton(btn);
		break;
	}
}

// Draw Player powerup effect timers
void DrawEffectTimers(Player& player)
{

	for (int i = 0; i < player.activeEffects.size(); i++)
	{
 		PowerUpEffect effect = player.activeEffects[i];
		Rectangle destRect = {GetScreenWidth() - 50.0f, 20.0f + (50.0f * i), 40.0f, 40.0f};
		switch (effect.type)
		{
		case PowerUpType::POWER:
			DrawTexturePro(GameAssets::powerUpSprites["Power"], {0.0f, 0.0f, (float)GameAssets::powerUpSprites["Power"].width, (float)GameAssets::powerUpSprites["Power"].height},
		  		destRect, {0.0f}, 0.0f, WHITE
		  	);
			break;
		case PowerUpType::SPEED:
			DrawTexturePro(GameAssets::powerUpSprites["Speed"], {0.0f, 0.0f, (float)GameAssets::powerUpSprites["Speed"].width, (float)GameAssets::powerUpSprites["Speed"].height},
		  		destRect, {0.0f}, 0.0f, WHITE
		  	);
			break;
		case PowerUpType::ONEUP:
			break;
                }
		float progress = effect.timeLeft / effect.totalTime;
		float angle = progress * 360.0f;

		Vector2 center = {destRect.x + destRect.width * 0.5f, destRect.y + destRect.height * 0.5f};
		float radius = 25.0f;
		Color timerColor = {130, 130, 130, 120}; 
		DrawCircleSector(center, radius,
		   	-90, -90 + angle, 100, timerColor);

       }
}

void InitUI(UIAssets& assets)
{
	Button restartBtn;
	restartBtn.text = "Restart";
	restartBtn.txtColor = RAYWHITE;
	restartBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) + 20.0f, 120, 50 };
	restartBtn.fontSize = 20;
	restartBtn.color = GRAY;
	assets.btns.insert({"Restart", restartBtn});

	Button startBtn;
	startBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) - 0.0f, 120, 50 };
	startBtn.color = DARKGRAY;
	startBtn.text = "Start Game";
	startBtn.txtColor = WHITE;
	startBtn.fontSize = 15;
	assets.btns.insert({"Start", startBtn});
}
