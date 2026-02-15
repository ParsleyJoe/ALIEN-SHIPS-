#include "button.hpp"
#include "raylib.h"
#include <UI.hpp>
#include <sceneManager.hpp>

void DrawUI(UIAssets& assets, Scene& currentScene)
{
	switch (currentScene)
	{
	case Scene::MAIN_MENU:
		DrawText("SPACE BULLET HELL!!", (GetScreenWidth() / 2) - 170, (GetScreenHeight() / 2) - 50, 30, DARKGRAY);
		DrawButton(assets.btns["Start"]);
		break;
	case Scene::GAME:
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

void InitUI(UIAssets& assets)
{
	Button restartBtn;
	restartBtn.text = "Restart";
	restartBtn.txtColor = RAYWHITE;
	restartBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) - 0.0f, 120, 50 };
	restartBtn.fontSize = 20;
	assets.btns.insert({"Restart", restartBtn});

	Button startBtn;
	startBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) - 0.0f, 120, 50 };
	startBtn.color = DARKGRAY;
	startBtn.text = "Start Game";
	startBtn.txtColor = WHITE;
	startBtn.fontSize = 15;
	assets.btns.insert({"Start", startBtn});
}
