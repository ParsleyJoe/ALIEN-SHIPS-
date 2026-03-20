#include "button.hpp"
#include "game.hpp"
#include "gameAssets.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "raylib.h"
#include <UI.hpp>
#include <sceneManager.hpp>

void DrawUI(UIAssets& assets, Game& game)
{
	static bool showGrid = false;
	if (IsKeyPressed(KEY_G))
		showGrid = !showGrid;

	if (showGrid)
		DrawGridLines(GetScreenWidth(), GetScreenHeight(), 32);
	switch (game.currentScene)
	{
	case Scene::MAIN_MENU:
		DrawMainMenu(assets, game.player);
		break;
	case Scene::GAME:
		DrawLives(game);
		DrawEffectTimers(game.player);
		DrawScore(game);
		break;
	case Scene::GAME_OVER:
		int textWidth = MeasureText("GAME OVER", 30);
		int x = (GetScreenWidth() / 2) - (textWidth / 2);
		int y = (GetScreenHeight() / 2) - (30 / 2); // optional, for vertical center
		DrawText("GAME OVER", x, y, 30, DARKGRAY);

		Button btn = assets.btns["Restart"];
		DrawButton(btn);
		btn = assets.btns["Menu"];
		DrawButton(btn);
		break;
	}
}

void DrawMainMenu(UIAssets& assets, Player& player)
{
	DrawCenteredText(GameAssets::gameFont, "Alien Ships", 60.0f, 40.0f, 0.0f, RAYWHITE);
	DrawButton(assets.btns["Start"]);

	float sizeIncrease = 40.0f;
	Texture2D& sprite = player.shipTextures[player.selectedShipIndex];
	// Draw Cycling of Ships
	DrawTexturePro(sprite, {0.0f, 0.0f, (float)sprite.width, (float)sprite.height},{(GetScreenWidth() / 2.0f) - ((sprite.width + sizeIncrease) / 2.0f),
	     (GetScreenHeight() / 2.0f) - sizeIncrease * 1.4f, (float)sprite.width + sizeIncrease, (float)sprite.height + sizeIncrease},{}, 0.0f, WHITE);

	DrawButton(assets.btns["LeftCycle"]);
	DrawButton(assets.btns["RightCycle"]);

	int fontSize = 30;
	Vector2 xLen = MeasureTextEx(GameAssets::gameFont,"Special: ", fontSize, 0.0f);
	float rightOffset = 60.0f;
	std::string special = "None:";

	DrawText("Special: ", GetScreenWidth() - rightOffset - xLen.x, (GetScreenHeight() / 2.0f) - (sizeIncrease * 1.4f) - 50.0f - xLen.y, fontSize, RAYWHITE);

	// Draw Special for selected ship
	switch (player.selectedShipIndex) 
	{
	case 0:
		xLen = MeasureTextEx(GameAssets::gameFont,"None", fontSize, 0.0f);
		special = "None";
		break;
	case 1:
		xLen = MeasureTextEx(GameAssets::gameFont,"Dodge Roll", fontSize, 0.0f);
		special = "Dodge Roll";
		break;
	case 2:
		xLen = MeasureTextEx(GameAssets::gameFont,"Extra Shots", fontSize, 0.0f);
		special = "Extra Shots";
		break;
	}
	DrawTextEx(GameAssets::gameFont, special.c_str(), {GetScreenWidth() - rightOffset - xLen.x, (GetScreenHeight() / 2.0f) - (sizeIncrease * 1.4f) - xLen.y}, fontSize,0.0f, RAYWHITE);
}

void DrawLives(Game &game)
{
	auto& liveTexture = game.player.shipTextures[0];
	float sizeMultiplier = 1.2f;
	DrawTexturePro(liveTexture, {0.0f, 0.0f, (float)liveTexture.width, (float)liveTexture.height}, {10, 10, liveTexture.width * sizeMultiplier, liveTexture.height * sizeMultiplier}, {}, 0.0f, WHITE);
	DrawTextPro(GameAssets::gameFont, TextFormat("%d", game.player.lives), {liveTexture.width * sizeMultiplier + 20, 10},{},0.0f, 34, 0.0f, RAYWHITE);
}

void DrawScore(Game& game)
{
	float size = MeasureText(TextFormat("%d", game.score), 20);
	DrawText(TextFormat("%d", game.score), (GetScreenWidth() * 0.5f) - size, 20.0f, 20, RAYWHITE);
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
		case PowerUpType::SHIELD:
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


	Button startBtn;
	startBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 60.0f, (GetScreenHeight() / 2.0f) + 90.0f, 120, 50 };
	startBtn.color = DARKGRAY;
	startBtn.text = "Play";
	startBtn.txtColor = WHITE;
	startBtn.fontSize = 15;
	assets.btns.insert({"Start", startBtn});

	Button restartBtn;
	restartBtn.text = "Restart";
	restartBtn.txtColor = RAYWHITE;
	restartBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) + 20.0f, 120, 50 };
	restartBtn.fontSize = 20;
	restartBtn.color = GRAY;
	assets.btns.insert({"Restart", restartBtn});

	Button menuBtn;
	menuBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 50.0f, (GetScreenHeight() / 2.0f) + 80.0f, 120, 50 };
	menuBtn.text = "Menu";
	menuBtn.txtColor = RAYWHITE;
	menuBtn.fontSize = 20;
	menuBtn.color = GRAY;
	assets.btns.insert({"Menu", menuBtn});
	

	// Cycling Buttons
	Button leftCycle;
	leftCycle.customSprite = LoadTexture("resources/cycleButton.png");
	leftCycle.rec = {(GetScreenWidth() / 2.0f) - 120.0f, (GetScreenHeight() / 2.0f) - 30, 50.0f, 40.0f};
	leftCycle.srcRec = {0.0f, 0.0f, -(float)leftCycle.customSprite.width, (float)leftCycle.customSprite.height};
	assets.btns.insert({"LeftCycle", leftCycle});

	Button rightCycle;
	rightCycle.customSprite = leftCycle.customSprite;
	rightCycle.rec = {(GetScreenWidth() / 2.0f) + 70.0f , (GetScreenHeight() / 2.0f) - 30, 50.0f, 40.0f};
	rightCycle.srcRec = {0.0f, 0.0f, (float)leftCycle.customSprite.width, (float)leftCycle.customSprite.height};
	assets.btns.insert({"RightCycle", rightCycle});
}

void UIUpdate(UIAssets &uiAssets, Game &game, std::vector<std::unique_ptr<Enemy>> &enemies)
{
	switch (game.currentScene)
	{
        case Scene::MAIN_MENU:
		if (IsButtonClicked(uiAssets.btns["Start"]))
			gRestartGame(game, enemies);


		if (IsButtonClicked(uiAssets.btns["LeftCycle"]))
			CycleSelectedShip(game.player, -1);
		if (IsButtonClicked(uiAssets.btns["RightCycle"]))
			CycleSelectedShip(game.player, 1);
		break;
        case Scene::GAME:
		break;
        case Scene::GAME_OVER:
		if (IsButtonClicked(uiAssets.btns["Restart"]))
			gRestartGame(game, enemies);
		if (IsButtonClicked(uiAssets.btns["Menu"]))
		{
			game.currentScene = Scene::MAIN_MENU;
		}
		break;
        }
}

void DrawCenteredText(Font font, const char* text, float y, float size, float spacing, Color color)
{
	Vector2 textSize = MeasureTextEx(font, text, size, spacing);
	DrawTextEx(
		font,
		text,
		{ GetScreenWidth() / 2.0f - textSize.x / 2.0f, y},
		size,
		spacing,
		color
	);
}

void DrawGridLines(int screenWidth, int screenHeight, int cellSize)
{
	// Vertical lines
	for (int x = 0; x <= screenWidth; x += cellSize)
	{
		DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
	}

	// Horizontal lines
	for (int y = 0; y <= screenHeight; y += cellSize)
	{
		DrawLine(0, y, screenWidth, y, LIGHTGRAY);
	}
}
