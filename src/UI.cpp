#include "button.hpp"
#include "game.hpp"
#include "gameAssets.hpp"
#include "player.hpp"
#include "powerup.hpp"
#include "raylib.h"
#include "safeSave.h"
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
		DrawButton(assets.btns["Settings"]);
		break;
	case Scene::GAME:
		DrawLives(game);
		DrawEffectTimers(game.player);
		DrawScore(game);
		DrawSpecialMeter(assets, game.player);
		if (!game.active)
		{
			// Gray panel
			DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), Fade(GRAY, 0.5f));

			DrawCenteredText(GameAssets::gameFont, "Paused", GetScreenHeight() * 0.45f, 40, 0.0f, LIGHTGRAY);
			DrawButton(assets.btns["Restart"]);
			DrawButton(assets.btns["Menu"]);
		}
		break;
	case Scene::GAME_OVER:
		DrawGameOver(assets, game);
		break;
	case Scene::SETTINGS:
		DrawButton(assets.btns["SettingsBack"]);
		DrawButton(assets.btns["LeftCycle"]);
		DrawButton(assets.btns["RightCycle"]);
		int vol = GetMasterVolume() * 10.0f;

		DrawCenteredText(GameAssets::gameFont, "Volume", GetScreenHeight() * 0.4f, 30, 0.0f, RAYWHITE);
		DrawCenteredText(GameAssets::gameFont, TextFormat("%d", vol), GetScreenHeight() * 0.45f, 30.0f, 0.0f, RAYWHITE);
		break;
	}
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

		if (IsButtonClicked(uiAssets.btns["Settings"]))
			game.currentScene = Scene::SETTINGS;

		break;
        case Scene::GAME:
		if (!game.active)
		{
			if (IsButtonClicked(uiAssets.btns["Restart"]))
				gRestartGame(game, enemies);
			if (IsButtonClicked(uiAssets.btns["Menu"]))
			{
				game.currentScene = Scene::MAIN_MENU;
				game.active = false;
			}
		}
		break;
        case Scene::GAME_OVER:
		if (IsButtonClicked(uiAssets.btns["Restart"]))
			gRestartGame(game, enemies);
		if (IsButtonClicked(uiAssets.btns["Menu"]))
		{
			game.currentScene = Scene::MAIN_MENU;
		}
		break;
        case Scene::SETTINGS:
		if (IsButtonClicked(uiAssets.btns["SettingsBack"]))
      			game.currentScene = Scene::MAIN_MENU;

		if (IsButtonClicked(uiAssets.btns["LeftCycle"]))
			SetMasterVolume(GetMasterVolume() - 0.1f);
		if (IsButtonClicked(uiAssets.btns["RightCycle"]))
			SetMasterVolume(GetMasterVolume() + 0.1f);

		break;
        }
}
void DrawGameOver(UIAssets& assets, Game& game)
{
	// NOTE: This code is for sickos
	int fontSize = 40;
	Vector2 textWidth = MeasureTextEx(GameAssets::gameFont, "GAME OVER", fontSize, 0.0f);
	int x = (GetScreenWidth() / 2.0f) - (textWidth.x / 2.0f);
	int y = (GetScreenHeight() * 0.18f) - (fontSize) - textWidth.y; // optional, for vertical center
	DrawTextEx(GameAssets::gameFont, "GAME OVER", {(float)x, (float)y}, fontSize, 0.0f, RAYWHITE);

	SaveData sav{};
	sfs::readEntireFileWithCheckSum((void*)&sav, sizeof(sav), GameAssets::saveFileName);
	const char* str = TextFormat("Highscore: %d", sav.score);
	textWidth = MeasureTextEx(GameAssets::gameFont, str, fontSize, 0.0f);
	DrawTextEx(GameAssets::gameFont, str, {(GetScreenWidth() * 0.5f) - (textWidth.x * 0.5f), (GetScreenHeight() * 0.5f) - textWidth.y}, fontSize, 0.0f, RAYWHITE);

	str = TextFormat("Score: %d", game.score);
	textWidth = MeasureTextEx(GameAssets::gameFont, str, fontSize, 0.0f);
	DrawTextEx(GameAssets::gameFont, str, {(GetScreenWidth() * 0.5f) - (textWidth.x * 0.5f), (GetScreenHeight() * 0.45f) - textWidth.y}, fontSize, 0.0f, RAYWHITE);

	Button btn = assets.btns["Restart"];
	DrawButton(btn);
	btn = assets.btns["Menu"];
	DrawButton(btn);

	Rectangle rect = {GetScreenWidth() * 0.14f, GetScreenHeight() * 0.14f, GetScreenWidth() * 0.7f, GetScreenHeight() * 0.7f};
	DrawTexturePro(assets.menuBackground, {0.0f, 0.0f, (float)assets.menuBackground.width, (float)assets.menuBackground.height},
			rect, {}, 0.0f, {255, 255, 255, 60});

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

	DrawText("Special: ", GetScreenWidth() - (rightOffset * 1.4f) - xLen.x, (GetScreenHeight() / 2.0f) - (sizeIncrease * 1.4f) - 50.0f - xLen.y, fontSize, RAYWHITE);

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
			DrawTexturePro(GameAssets::powerUpSprites["Shield"], {0.0f, 0.0f, (float)GameAssets::powerUpSprites["Shield"].width, (float)GameAssets::powerUpSprites["Shield"].height},
		  		destRect, {0.0f}, 0.0f, WHITE
		  	);
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

	assets.menuBackground = LoadTexture("resources/menuBackground.png");

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
	restartBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 60.0f, (GetScreenHeight() / 2.0f) + 20.0f, 120, 50 };
	restartBtn.fontSize = 20;
	restartBtn.color = GRAY;
	assets.btns.insert({"Restart", restartBtn});

	Button menuBtn;
	menuBtn.rec = Rectangle{ (GetScreenWidth() / 2.0f) - 60.0f, (GetScreenHeight() / 2.0f) + 80.0f, 120, 50 };
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

	Button settingsBtn;
	settingsBtn.rec = {GetScreenWidth() - 50.0f, GetScreenHeight() - 50.0f, 30.0f, 30.0f};
	settingsBtn.customSprite = GameAssets::powerUpSprites["OneUp"];
	settingsBtn.srcRec = {0.0f, 0.0f, (float)settingsBtn.customSprite.width, (float)settingsBtn.customSprite.height};
	assets.btns.insert({"Settings", settingsBtn});

	Button settingsBackBtn;
	settingsBackBtn.rec = {GetScreenWidth() - 50.0f, GetScreenHeight() - 50.0f, 30.0f, 30.0f};
	settingsBackBtn.customSprite = rightCycle.customSprite;
	settingsBackBtn.srcRec = rightCycle.srcRec;
	assets.btns.insert({"SettingsBack", settingsBackBtn});
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

void DrawSpecialMeter(UIAssets& assets, Player& player)
{
	if (player.selectedShipIndex == 0)
		return;
	float specialPercentage = (float)player.specialMeter / (float)player.specialFullLevel;
	DrawRectangle(20.0f, 60.0f, 100.0f * specialPercentage, 30.0f, BLUE);
}

