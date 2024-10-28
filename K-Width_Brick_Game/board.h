#pragma once
#include <QtWidgets>
#include "tile.h"

//Avoiding magic numbers, in future move to Defines.h file
#define Game_Area_Height 20
#define Game_Area_Width 10

// GAME WINDOW DATA:
const int gameWindowHeight = 480;	// height of the game window (in pixels)
const int gameWindowWidth = 320;	// width of the game window (in pixels)


class board
{
public:
	tile* gameArea[Game_Area_Height][Game_Area_Width];	// the game area is 20 tiles high and 10 tiles wide
	board(QGraphicsScene* scene, QGraphicsView* view);

	void changeTile(int i, int j, int colour);
	bool isTileTaken(int i, int j);
	void resetTile(int i, int j);
	void changeTileStatus(int i, int j, bool b);

	friend class tile;
};

