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

	int colour;	// sprite's colour
	int shape; // sprite's shape
	int rotation; // (all rotations are done clockwise): 0 - default, 1 - 90deg, 2 - 180deg, 3 - 270deg

	brick(int col, int sh);

	void draw(board* b);
};

