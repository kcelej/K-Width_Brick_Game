#pragma once
#include "board.h"

/*
 * Represents a Tetris brick.
 * @details A brick is a 2x4 structure composed of individual tiles.
 *          It has a specific shape, color, rotation state, and
 *          movement behavior. The brick interacts with the game board
 *          and can be moved, rotated, and checked for collisions.
 */
class brick{
public:
	/*
	 * A brick is essentially a 2x4 sprite, with tiles being drawn wherever
	 * the boolean value is `1`. It also has a set of coordinates for each tile
	 * to track movement.
	 *
	 * Brick tile layout:
	 * [0][1]
	 * [2][3]
	 * [4][5]
	 * [6][7]
	 */

	 /*
	  * Brick rotation states:
	  *
	  * Rotation = 0 (default)
	  * [0][1]
	  * [2][3]
	  * [4][5]
	  * [6][7]
	  *
	  * Rotation = 1 (90° clockwise)
	  * [6][4][2][0]
	  * [7][5][3][1]
	  *
	  * Rotation = 2 (180°)
	  * [7][6]
	  * [5][4]
	  * [3][2]
	  * [1][0]
	  *
	  * Rotation = 3 (270° clockwise)
	  * [1][3][5][7]
	  * [0][2][4][6]
	  */

	/*
	 * Represents a point in a 2D grid.
	 * @details This struct holds row (`i`) and column (`j`) coordinates
	 *          used to track the position of individual tiles within a brick.
	 */
	struct point {
		int i;
		int j;

		/*
		 * Default constructor initializes the point at (0,0).
		 */
		point() : i(0), j(0) {}

		/*
		 * Copies coordinates from another point.
		 * @param old The point to copy coordinates from.
		 * @return A reference to the updated point.
		 */
		point& copy_coordinates(const point& old) {
			this->i = old.i;
			this->j = old.j;
			return *this;
		}
	};	// Basic coordinates initialized to (0,0), updated in the constructor.

	const bool shapes[7][8]{
		// I-shape: A vertical line of 4 blocks
		{
			1, 0,
			1, 0,
			1, 0,
			1, 0
		},

		// O-shape: A 2x2 square block
		{
			0, 0,
			0, 0,
			1, 1,
			1, 1
		},

		// J-shape: L-shape extending left
		{
			0, 0,
			0, 1,
			0, 1,
			1, 1
		},

		// L-shape: L-shape extending right
		{
			0, 0,
			1, 0,
			1, 0,
			1, 1
		},

		// S-shape: A left-leaning diagonal curve
		{
			0, 0,
			1, 0,
			1, 1,
			0, 1
		},

		// T-shape: A central block with three connected tiles
		{
			0, 0,
			0, 1,
			1, 1,
			0, 1
		},

		// Z-shape: A right-leaning diagonal curve
		{
			0, 0,
			0, 1,
			1, 1,
			1, 0
		}
	};

	/*
	 * Constructor for the `brick` class.
	 * @details Initializes a new brick with a given color and shape, and sets its initial position above the game board.
	 * @param col The color of the brick.
	 * @param sh The shape identifier of the brick.
	 * @param _b Pointer to the game board.
	 */
	brick(int col, int sh, board* _b);

	/*
	 * Destructor for the `brick` class.
	 * @details Resets the board pointer to prevent dangling references.
	 */
	~brick() { b = nullptr; }

	/*
	 * Draws the brick on the game board.
	 * @details Iterates through all brick parts, checking if they are visible and within bounds.
	 *          If valid, updates the game board with the brick's color and marks tiles as occupied.
	 */
	void draw() const;

	/*
	 * Clears the entire brick from the game board.
	 * @details Resets all tiles occupied by the brick, making them empty again.
	 */
	void reset_entire_brick() const;

	/*
	 * Handles player input to move or rotate the brick.
	 * @param event A pointer to the `QKeyEvent` containing the key press information.
	 */
	void movement(QKeyEvent* event);

	/*
	 * Checks if moving the brick in a given direction will cause a collision.
	 * @param direction The movement direction (0 = Down, 1 = Left, 2 = Right).
	 * @return `true` if a collision is detected, `false` otherwise.
	 */
	bool collision (int direction) const;

	/*
	 * Checks if the brick has exceeded the upper boundary of the board, causing a game-over condition.
	 * @return `true` if the game is lost, `false` otherwise.
	 */
	bool chceck_for_defeat();

	// Stores the coordinates of each tile in the brick.
	point coordinates[ALL_TILES];

	// Indicates whether the brick can continue moving.
	bool can_be_still_moved = true;

	// Returns the brick's color.
	int get_color() const { return color; }

	// Checks if a tile is present in the shape.
	bool get_tile_presence(const int tile) const { return shapes[shape][tile]; }

	void move_down_by_game();
private:
	// The color of the brick.
	int color;
	// The shape identifier of the brick.
	int shape;
	// Rotation state: 0 (default), 1 (90°), 2 (180°), 3 (270°).
	int rotation;
	// Pointer to the game board.
	board* b = nullptr;

	/*
	 * Rotates the brick if possible.
	 * @details Increments the rotation state and checks for collisions.
	 *          If a collision occurs, the rotation is reverted.
	 */
	void change_rotation();

	/*
	 * Attempts to rotate the brick to the next state.
	 * @return `true` if rotation is successful, `false` if blocked by a collision.
	 */
	bool rotate();

	/*
	 * Rotates the brick to its default orientation.
	 * @param Temp_coordinates Temporary storage for new brick positions.
	 */
	void rotateDefault(point* Old_coordinates);

	/*
	 * Rotates the brick 90 degrees clockwise.
	 * @param Temp_coordinates Temporary storage for new brick positions.
	 */
	void rotate90Degrees(point* Old_coordinates);

	/*
	 * Rotates the brick 180 degrees.
	 * @param Temp_coordinates Temporary storage for new brick positions.
	 */
	void rotate180Degrees(point* Old_coordinates);

	/*
	 * Rotates the brick 270 degrees (or -90 degrees).
	 * @param Old_coordinates Temporary storage for new brick positions.
	 */
	void rotate270Degrees(point* Old_coordinates);

	/*
	 * Checks if rotating the brick would cause a collision.
	 * @param temp_coords The new coordinates after the rotation.
	 * @return `true` if a collision is detected, `false` otherwise.
	 */
	bool checkForCollision_Rotation(const point* temp_coords) const;

	/*
	 * Checks if the brick will collide with another brick.
	 * @param index The index of the brick part being checked.
	 * @param offset_i Vertical movement offset.
	 * @param offset_j Horizontal movement offset.
	 * @return `true` if a collision with another brick is detected, `false` otherwise.
	 */
	bool checkForCollision_Movement_Brick(int index, int offset_i, int offset_j) const;

	/*
	 * Checks if the brick will collide with the game area's borders.
	 * @param index The index of the brick part being checked.
	 * @param offset_i Vertical movement offset.
	 * @param offset_j Horizontal movement offset.
	 * @return `true` if a border collision is detected, `false` otherwise.
	 */
	bool checkForCollision_Movement_Border(int index, int offset_i, int offset_j) const;

	/*
	 * Increments the rotation index, cycling between 0 and 3.
	 */
	void increment_rotation();

	/*
	 * Moves the brick down if possible.
	 * @return `true` if the brick was moved, `false` if it hit an obstacle.
	 */
	bool move_down();

	/*
	 * Moves the brick to the left if possible.
	 */
	void move_left();

	/*
	 * Moves the brick to the right if possible.
	 */
	void move_right();
};

