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
};

bool IsButtonClicked(Button& button);
void DrawButton(Button& button);
