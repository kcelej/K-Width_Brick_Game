#pragma once
#include <QtWidgets>
#include "tile.h"
#include "NetworkMessageManager.h"
#include <mutex>

// GAME WINDOW DATA:
const int gameWindowHeight = 480;	// height of the game window (in pixels)
const int gameWindowWidth = 320;	// width of the game window (in pixels)

/*
 * Represents the game board for a Tetris-like game.
 * @details The board is a grid of tiles where the bricks are placed.
 *          It handles tile updates, row clearing, and movement logic.
 *          The class also integrates network messaging for multiplayer functionality.
 */
class board {
private:
	NetworkMessageManager* networkMessageManager = nullptr;
	void notifyOtherNetworkPlayers(NetworkMessage message);
	bool linesStatus[GAME_AREA_HEIGHT];
	void setLineStatus(int lineNumber, bool checked);
	std::mutex boardMutex;
	/// Pointer to the graphical view displaying the board.
	QGraphicsView* gameView = nullptr;
public:
	/*
	 * Represents the game grid.
	 * The game board consists of `GAME_AREA_HEIGHT` rows and `GAME_AREA_WIDTH` columns.
	 * Each cell is a pointer to a `tile` object.
	 */
	tile* gameArea[GAME_AREA_HEIGHT][GAME_AREA_WIDTH];
	
	/*
	 * Constructor for the `board` class.
	 * @details Initializes the game board by creating a grid of tiles and adding them to the scene.
	 * @param scene Pointer to the QGraphicsScene where the tiles are displayed.
	 */
	board(QGraphicsScene* scene);

	/*
	 * Changes the color of a specific tile on the board.
	 * @param i Row index of the tile.
	 * @param j Column index of the tile.
	 * @param color The new color index to apply.
	 */
	void changeTile(int i, int j, int color);

	/*
	 * Checks if a specific tile is occupied.
	 * @param i Row index of the tile.
	 * @param j Column index of the tile.
	 * @return `true` if the tile is occupied, `false` otherwise.
	 */
	bool isTileTaken(int i, int j);

	/*
	 * Resets a tile to its default empty state.
	 * @param i Row index of the tile.
	 * @param j Column index of the tile.
	 */
	void resetTile(int i, int j);

	/*
	 * Changes the occupancy status of a tile.
	 * @param i Row index of the tile.
	 * @param j Column index of the tile.
	 * @param b `true` if the tile is occupied, `false` if it is empty.
	 */
	void changeTileStatus(int i, int j, bool b);
	
	/*
	 * Checks if a line is fully occupied or fully empty.
	 * @param i Row index to check.
	 * @param check_for_occupied If `true`, checks if the row is fully occupied. If `false`, checks if it is fully empty.
	 * @return `true` if the condition is met, `false` otherwise.
	 */
	bool check_line_condition(int i, bool check_for_occupied);

	/*
	 * Deletes a full row by resetting all its tiles.
	 * @param i The index of the row to delete.
	 */
	void delete_line(int i);

	/*
	 * Moves all rows above the given row down by one position.
	 * @param i The row index where the shift begins.
	 */
	void move_all_down(int i);

	/*
	 * Checks the board for completed rows and processes them accordingly.
	 */
	void check_board();

	void setNetworkMessageManager(NetworkMessageManager* networkMessageManagerVal);

	/*
	 * Sets the game view reference.
	 * @param view Pointer to the QGraphicsView associated with the board.
	 */
	void setGameView(QGraphicsView*);

	/// Grants the `tile` class access to private members of `board`.
	friend class tile;
};

