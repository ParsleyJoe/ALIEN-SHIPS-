#pragma once
#include <raylib.h>
#include <string>

struct Button
{
	Rectangle rec;
	Color color;
	std::string text;
	Color txtColor;
	int fontSize;

	// Optional Sprite
	Texture2D customSprite = {0};
	Rectangle srcRec = {0};
};

bool IsButtonClicked(Button& button);
void DrawButton(Button& button);
