#include "hud.hpp"
#include "gameAssets.hpp"

void DrawHealthBar(HealthBar& healthBar, int startHealth)
{
	//DrawRectangleRec(healthBar.mainRec, GRAY);
	float percentHealth = (healthBar.health / static_cast<float>(startHealth));
	DrawRectangle(healthBar.fillRec.x, healthBar.fillRec.y, healthBar.fillRec.width, healthBar.fillRec.height * percentHealth, RED);
	DrawTexture(GameAssets::bossHealthBorder, healthBar.mainRec.x, healthBar.mainRec.y, PURPLE);
}