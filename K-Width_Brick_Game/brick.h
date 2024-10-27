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
	} coordinates[8];	// coordinates of each tile, -1 is the default for "not shown on screen"

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
	bool collision(int direction);	// directions: 0 - down, 1 - left, 2 - right

	/*
	* Checks collisions only with downward movement of block 
	* @param min lower range to for loop
	* @param max upper range to for loop
	* @return Return true if collision detected, false otherwise 
	*/
	bool collision_down(int min, int max); 

private:
	int colour;	// sprite's colour
	int shape; // sprite's shape
	int rotation; // (all rotations are done clockwise): 0 - default, 1 - 90deg, 2 - 180deg, 3 - 270deg
	board* b = nullptr; // pointer to board

	/*
	* Method checks possibility of changing brick rotation
	* @return True if rotation is possible, otherwise false 
	*/
	bool rotate();
};

