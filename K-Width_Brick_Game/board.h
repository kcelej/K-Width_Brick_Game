#pragma once
#include <QtWidgets>
#include "tile.h"
#include "NetworkMessageManager.h"
#include <mutex>
// GAME WINDOW DATA:
const int gameWindowHeight = 480;	// height of the game window (in pixels)
const int gameWindowWidth = 320;	// width of the game window (in pixels)


class board
{
private:
	NetworkMessageManager* networkMessageManager = nullptr;
	void notifyOtherNetworkPlayers(NetworkMessage message);
	bool linesStatus[GAME_AREA_HEIGHT];
	void setLineStatus(int lineNumber, bool checked);
	std::mutex boardMutex;
public:
	tile* gameArea[GAME_AREA_HEIGHT][GAME_AREA_WIDTH];	// the game area is 20 tiles high and 10 tiles wide
	board(QGraphicsScene* scene);

	void changeTile(int i, int j, int color);
	bool isTileTaken(int i, int j);
	void resetTile(int i, int j);
	void changeTileStatus(int i, int j, bool b);

	/*
	 * Checks whether a specific line in the game area meets a given condition (fully occupied or fully empty).
	 *
	 * @param i The index of the line (row) to check in the game area.
	 * @param check_for_occupied A boolean that determines the condition to check:
	 *                           - true: Checks if the line is fully occupied.
	 *                           - false: Checks if the line is fully empty.
	 * @return Returns true if the line satisfies the specified condition; false otherwise.
	 */
	bool check_line_condition(int i, bool check_for_occupied);

	/*
	 * Deletes all tiles in a specific line (row) of the game area.
	 * @param i The index of the line (row) to delete in the game area.
	 *        All tiles in this line will have their status changed to unoccupied.
	 */
	void delete_line(int i);

	/*
	 * Moves all rows on the board down starting from a specified row.
	 * This process shifts the contents of rows downward, effectively clearing the specified row and replacing it with the contents of the row above.
	 * If there is no row above (at the top of the board), the current row is deleted.
	 *
	 * @param i The index of the starting row from which to begin shifting rows downward.
	 *          All rows from this index up to the top of the board are affected.
	 */
	void move_all_down(int i);

	/*
	 * Checks the entire game board for fully occupied lines and removes them.
	 * Lines are processed from the bottom to the top of the board.
	 * If a line is fully occupied, it is deleted, and all lines above it are shifted down.
	 */
	void check_board();

	void setNetworkMessageManager(NetworkMessageManager* networkMessageManagerVal);

	friend class tile;
};

