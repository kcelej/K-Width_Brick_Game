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

	/*
	 * Checks if a specific line in the game area is fully occupied.
	 * @param i The index of the line (row) to check in the game area.
	 * @return Returns true if the line is completely filled with occupied cells; false otherwise.
	 */
	bool check_line(int i);

	/*
	 * Deletes all tiles in a specific line (row) of the game area.
	 * @param i The index of the line (row) to delete in the game area.
	 *        All tiles in this line will have their status changed to unoccupied.
	 */
	void delete_line(int i);


	void move_all_down(int i);

	/*
	 * Checks the entire game board for fully occupied lines and removes them.
	 * Lines are processed from the bottom to the top of the board.
	 * If a line is fully occupied, it is deleted, and all lines above it are shifted down.
	 */
	void check_board();


	friend class tile;
};

