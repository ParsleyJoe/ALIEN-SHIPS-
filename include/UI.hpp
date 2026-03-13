#pragma once
#include "button.hpp"
#include <map>
#include <memory>
#include <vector>

enum class Scene;
struct Player;
struct Game;
struct Enemy;

struct UIAssets {
	std::map<std::string, Button> btns;
	Font defaultFont;
};

void DrawUI(UIAssets& assets, Scene& currentScene, Game& game);
void InitUI(UIAssets& assets);
void DrawEffectTimers(Player& player);
void DrawScore(Game& game);
void UIUpdate(Scene &currentScene, UIAssets &uiAssets, Game &game, std::vector<std::unique_ptr<Enemy>> &enemies);
