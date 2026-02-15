#pragma once
#include "button.hpp"
#include <map>

enum class Scene;

struct UIAssets {
	std::map<std::string, Button> btns;
	Font defaultFont;
};

void DrawUI(UIAssets& assets, Scene& currentScene);
void InitUI(UIAssets& assets);
