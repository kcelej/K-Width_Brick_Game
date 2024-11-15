#pragma once
#include "board.h"

class brick
{
public:
	/*
	a brick is essentially a 2x4 sprite with tiles being printed wherever the boolean value is equal to 1 and a set of coordinates for each tile for movement tracking
	[0][1]
	[2][3]
	[4][5]
	[6][7]
	*/
	/*
	Brick rotation
		Rotation = 0
		[0][1]
		[2][3]
		[4][5]
		[6][7]

		Rotation = 1
		[6][4][2][0]
		[7][5][3][1]

		Rotation = 2
		[7][6]
		[5][4]
		[3][2]
		[1][0]

		Rotation = 3
		[1][3][5][7]
		[0][2][4][6]
	*/

	struct point {
		int i;
		int j;

		point() : i(0), j(0) {}

		point copy_coordinates(point* old) {
			this->i = old->i;
			this->j = old->j;
			return *this;
		}
	};	// basic coordinates i = 0, j = 0, changed later in the brick constructor to invisible in the game area

	const bool shapes[7][8]{
		{1,0,
		 1,0,
		 1,0,
		 1,0},	// I
			{0,0,
		 	 0,0,
			 1,1,
			 1,1},	// O
				{0,0,
				 0,1,
				 0,1,
				 1,1},	// J
					{0,0,
					 1,0,
					 1,0,
					 1,1},	// L
						{0,0,
						 1,0,
						 1,1,
						 0,1},	// S
							{0,0,
							 0,1,
							 1,1,
							 0,1},	// T
								{0,0,
								 0,1,
								 1,1,
								 1,0}	// Z
	};


	brick(int col, int sh, board* _b);
	~brick() {
		b = nullptr;
	}

	/*
	 * Draws the brick on the game area grid by setting each part of the brick to its specified color and updating its status.
	 * @details This function iterates over each part of the current shape and checks if the part is active.
	 *          If a part is active and has valid coordinates (i.e., both vertical and horizontal coordinates are non-negative),
	 *          it sets the tile at the specified position to the brick's color and marks the tile as occupied.
	 */
	void draw();

	/*
	 * Resets all parts of the current brick in the game area.
	 * @details This function iterates over each part of the brick and checks if its coordinates
	 *          are valid (i.e., both vertical and horizontal coordinates are non-negative).
	 *          If the part is part of the current shape and occupies a valid position in the game area,
	 *          it calls `resetTile` to clear the corresponding tile.
	 */
	void reset_entire_brick();

	/*
	* Main idea
	* Moves the block in the appropriate direction depending on which event in the QKeyEvent occurred
	* Possible movements left, right, down
	* True if another move possible
	* False if touched smt under
	*/
	bool movement(QKeyEvent* event);

	//for test only, delete later
	bool movement(int key);

	//after the tests, move to private
	/*
	* Method changes the value of the brick rotation field
	* @param new_rotation new rotation we want to set
	*/
	void change_rotation();

	/*
	* The collision method checks for potential collisions between a brick and other elements 
	* on the game board when the brick moves in a specific direction.
	* @param direction integer value that tells which way the block is moving
	* @return Return true if collision detected, false otherwise 
	*/
	bool collision (int direction);	// directions: 0 - down, 1 - left, 2 - right

	//after the tests, move to private
	/*
	 * Attempts to move the brick down by one block if no collision is detected.
	 * @details This function first checks for potential collisions with the game area border or other blocks.
	 *          If no collision is found, it increments the vertical position of each part of the brick by one.
	 * @return Returns true if the brick was successfully moved down; returns false if a collision was detected and the brick cannot move further.
	 */
	bool move_down();

	//after the tests, move to private
	/*
	 * Attempts to move the brick one block to the left if no collision is detected.
	 * @details This function checks for potential collisions on the left side of the brick.
	 *          If no collision is detected, it decrements the horizontal position of each part by one.
	 */
	void move_left();

	//after the tests, move to private
	/*
	 * Attempts to move the brick one block to the right if no collision is detected.
	 * @details This function checks for potential collisions on the right side of the brick.
	 *          If no collision is detected, it increments the horizontal position of each part by one.
	 */
	void move_right();

	point coordinates[8];
private:
	int colour;	// sprite's colour
	int shape; // sprite's shape
	int rotation; // (all rotations are done clockwise): 0 - default, 1 - 90deg, 2 - 180deg, 3 - 270deg
	board* b = nullptr; // pointer to board

	/*
	* Rotates the brick based on the current rotation state.
	* Checks for boundaries and collisions before performing the rotation.
	* @return Return true if rotation is possible and completed successfully, false otherwise.
	*/
	bool rotate();

	/*
	* Rotates the coordinates to default position.
	* @param Old_coordinates Array of old coordinates before rotation.
	*/
	void rotateDefault(point* Old_coordinates);

	/*
	* Rotates the coordinates 90 degrees clockwise.
	* @param Old_coordinates Array of old coordinates before rotation.
	*/
	void rotate90Degrees(point* Old_coordinates);

	/*
	* Rotates the coordinates 180 degrees.
	* @param Old_coordinates Array of old coordinates before rotation.
	*/
	void rotate180Degrees(point* Old_coordinates);

	/*
	* Rotates the coordinates 270 degrees counter-clockwise (or 90 degrees clockwise).
	* @param Old_coordinates Array of old coordinates before rotation.
	*/
	void rotate270Degrees(point* Old_coordinates);

	/*
	 * Checks for collision based on the temporary coordinates of the brick after rotation.
	 * @param temp_coords Temporary coordinates of the brick after rotation.
	 * @return Returns true if any part of the brick is visible and collides with another block or the board boundary; false otherwise.
	 */
	bool checkForCollision_Rotation(const point* temp_coords) const;

	bool checkForCollision_Movement_Brick(int index, int offset_i, int offset_j) const;

	bool checkForCollision_Movement_Border(int index, int offset_i, int offset_j) const;

	/*
	* Increments the rotation state of the brick, cycling between 0 and 3.
	* @details The brick rotation is represented as an integer from 0 to 3,
	*          with each value corresponding to a specific orientation.
	*          If the current rotation is less than 3, it increments by 1;
	*          otherwise, it resets to 0, completing the cycle.
	*/
	void increment_rotation();
};

