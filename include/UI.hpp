#pragma once
#include "button.hpp"
#include <map>

enum class Scene;
struct Player;

struct UIAssets {
	std::map<std::string, Button> btns;
	Font defaultFont;
};

void DrawUI(UIAssets& assets, Scene& currentScene, Player& player);
void InitUI(UIAssets& assets);
void DrawEffectTimers(Player& player);
