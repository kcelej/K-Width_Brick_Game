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

	struct point {
		int i;
		int j;

		point() : i(0), j(0) {}

		point copy_old_coordinates(point* old) {
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
	~brick() { b = nullptr; }
	void draw();

	void reset_entire_brick();

	/*
	* Main idea
	* Moves the block in the appropriate direction depending on which event in the QKeyEvent occurred
	* Possible movements left, right, down
	*/
	void movement(QKeyEvent* event);

	/*
	* Method changes the value of the brick rotation field
	* @param new_rotation new rotation we want to set
	*/
	void set_rotation(int new_rotation);

	/*
	* The collision method checks for potential collisions between a brick and other elements 
	* on the game board when the brick moves in a specific direction.
	* @param direction integer value that tells which way the block is moving
	* @return Return true if collision detected, false otherwise 
	*/
	bool collision (int direction) const;	// directions: 0 - down, 1 - left, 2 - right

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
	 * Checks boundaries for the current rotation and resets the tiles.
	 * @return Return true if rotation is possible and tiles are reset, false otherwise.
	*/
	bool checkBoundariesAndReset();

	/*
	* Checks for other blocks in the game area which may already occupy the space we are checking
	* @return Return true if no collision with other blocks is detected, false otherwise.
	*/
	bool checkForOtherBlocks();

	/*
	* Checks collisions only with downward movement of block
	* @param min lower range to for loop
	* @param max upper range to for loop
	* @return Return true if collision detected, false otherwise
	*/
	bool collision_down(int min, int max) const;

	bool collision_left(int min, int max) const;

	bool collision_right(int min, int max) const;
};

