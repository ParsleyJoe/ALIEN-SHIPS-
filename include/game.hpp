#pragma once

struct ImGuiIO;

struct Game {
	ImGuiIO* io;
};

void InitGame(Game& game);

// InitEverything before drawing
void gDrawingBegin();
void gDrawingEnd(Game& game);
