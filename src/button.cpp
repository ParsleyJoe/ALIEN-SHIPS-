#include "button.hpp"
#include <raylib.h>

bool IsButtonClicked(Button& button)
{
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
	{
		Vector2 mousePos = GetMousePosition();
		return CheckCollisionPointRec(mousePos, button.rec);
	}
	return false;
}

void DrawButton(Button& button)
{
	DrawRectangleRec(button.rec, button.color);
	int textWidth = MeasureText(button.text.c_str(), button.fontSize);
	int textX = button.rec.x + (button.rec.width - textWidth) / 2;
	int textY = button.rec.y + (button.rec.height - button.fontSize) / 2; // fontSize approximates height
	DrawText(button.text.c_str(), textX, textY, button.fontSize, button.txtColor);//	DrawText(button.text.c_str(), x, y, button.fontSize, button.txtColor);
}