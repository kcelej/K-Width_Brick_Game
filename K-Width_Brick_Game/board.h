#pragma once
#include <QtWidgets>

#include "tile.h"

// GAME WINDOW DATA:
const int gameWindowHeight = 480;	// height of the game window (in pixels)
const int gameWindowWidth = 320;	// width of the game window (in pixels)


class board
{
private:
	tile* gameArea[20][10];	// the game area is 20 tiles high and 10 tiles wide

public:
	board(QGraphicsScene* scene, QGraphicsView* view);

	void changeTile(int i, int j, int colour);

	friend class tile;
};

