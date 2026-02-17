#pragma once
#include "button.hpp"
#include <map>

enum class Scene;
struct Player;
struct Game;

struct UIAssets {
	std::map<std::string, Button> btns;
	Font defaultFont;
};

void DrawUI(UIAssets& assets, Scene& currentScene, Game& game);
void InitUI(UIAssets& assets);
void DrawEffectTimers(Player& player);
void DrawScore(Game& game);
