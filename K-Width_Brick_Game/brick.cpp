#include "brick.h"


brick::brick(int col, int sh, board* _b) : colour(col), shape(sh), rotation(0), b(_b) {
	for (int i = 0; i < 8; i++) {
		coordinates[i].i = -1;
		coordinates[i].j = -1;
	}
}

void brick::draw() {
	for (int i = 0; i < 8; i++) {
		if (shapes[shape][i] != 0) {
			if (coordinates[i].i != -1 || coordinates[i].j != -1) {
				b->changeTile(coordinates[i].i, coordinates[i].j, this->colour);
				//b->changeTileStatus(coordinates[i].i, coordinates[i].j, 1);
			}
		}
	}
}

void brick::set_rotation(int new_rotation) {
	int old_rotation = rotation;
	rotation = new_rotation;
	//if the rotation does not cause any collisions rotate
	if (rotate()) return;
	else {
		//if yes then set the old rotation value
		rotation = old_rotation;
		return;
	}
}

bool brick::rotate() {
	if (rotation == 0) {
		/*
		basic idea
		from
		[1][3][5][7]
		[0][2][4][6]

		to
				[0][1]
				[2][3]
				[4][5]
				[6][7]
		*/
		//check if rotation can occur?
		/*if () {
			return 0;
		}
		else{*/

		//rotate
		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd line then take y coordinates from 7, if even then y coordinates 6
			int y = coordinates[6].i;
			//If we go odd line then take x coordinates from 6, if even then x coordinates 4
			int x = coordinates[(i % 2) ? 6 : 4].j;
			while (k >= i - 6) {
				//x coordinates doesn't change only y
				coordinates[k].i = y--;
				coordinates[k].j = x;
				k -= 2;
			}
		}
		// Rotation possible and done, return true
		return 1;
	}

	else if (rotation == 1) {
		/*
		basic idea
		from
		[0][1]
		[2][3]
		[4][5]
		[6][7]

		to 
		[6][4][2][0]
		[7][5][3][1]
		*/

		//check if free space is available?
		/*if () {
			return 0;
		}
		else{*/

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = coordinates[(i % 2) ? 7 : 4].i;
			int x = coordinates[6].j;
			while (k >= i - 6) {
				//y coordinates doesn't change only x
				coordinates[k].i = y;
				coordinates[k].j = x++;
				k -= 2;
			}
		}

		// Rotation possible and done, return true
		return 1;
	}

	if (rotation == 2) { // 180 degrees
		/*
		basic idea
		from
		[6][4][2][0]
		[7][5][3][1]

		to
		[7][6]
		[5][4]
		[3][2]
		[1][0]
		*/

		//check if rotation can occur?
		/*if () {
			return 0;
		}
		else{*/

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = coordinates[6].i;
			int x = coordinates[(i % 2) ? 6: 4].j;
			while (k >= i - 6) {
				//x coordinates doesn't change only y
				coordinates[k].i = y++;
				coordinates[k].j = x;
				k -= 2;
			}
		}

		// Rotation possible and done, return true
		return 1;
	}

	else {	// rotation == 3
		/*
		basic idea
		from
		[7][6]
		[5][4]
		[3][2]
		[1][0]

		to
		[1][3][5][7]
		[0][2][4][6]
		*/

		//check if rotation can occur?
		/*if () {
			return 0;
		}
		else{*/

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = coordinates[6].i;
			int x = coordinates[(i % 2) ? 6 : 4].j;
			while (k >= i - 6) {
				//y coordinates doesn't change only x
				coordinates[k].i = y;
				coordinates[k].j = x--;
				k -= 2;
			}
		}
		// Rotation possible and done, return true
		return 1;
	}
}


bool brick::collision(int direction) {	// direction of falling: 0 - down, 1 - left, 2 - right
	if (direction == 1) {

	}
	else if (direction == 2) {

	}
	else {	// direction == 0
		if (rotation == 0) {
			//delete in the future
			//// Loop through indices 6 and 7 to check the corresponding shapes and coordinates
			//for (int i = 6; i <= 7; i++) {
			//	int k = i;
			//	// Check until reaching a relevant index (0 or 1)
			//	while (k >= i - 6) {
			//		// Check if the shape at the current index has a value of 1
			//		if (shapes[shape][k] == 1) {
			//			// Ensure that the next row (i + 1) is within the game area boundary
			//			if (coordinates[k].i + 1 < 20) { //bottom edge
			//				// Check if the position in the next row is occupied
			//				if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
			//					// Return 1 if the position is occupied (collision detected)
			//					return 1;
			//				}
			//			}
			//			else {
			//				return 1;
			//			}
			//			// Exit the loop if a block is found and no collision detected
			//			break;
			//		}
			//		k -= 2; // Skip to the next relevant index if no block at current index
			//	}
			//}
			//// If no collision is detected, return 0 (no collision)
			//return 0;

			//Returns 1 if collision is detected, 0 if not
			return collision_down(6, 7);
		}

		else if (rotation == 1) { //90deg
			// Loop through indices 1, 3, 5 and 7 to check the corresponding shapes and coordinates
			for (int i = 1; i <= 7; i += 2) {
				int k = i;
				// Check until reaching a relevant index (0 or 1)
				while (k >= i - 1) {
					// Check if the shape at the current index has a value of 1
					if (shapes[shape][k] == 1) {
						// Ensure that the next row (i + 1) is within the game area boundary
						if (coordinates[k].i + 1 < 20) { //bottom edge
							// Check if the position in the next row is occupied
							if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
								// Return 1 if the position is occupied (collision detected)
								return 1;
							}
						}
						else {
							return 1;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					--k; // Skip to the next relevant index if no block at current index
				}
			}
			// If no collision is detected, return 0 (no collision)
			return 0;

			//return collision_down(1, 7);
		}

		else if (rotation == 2) { // 180 degrees rotation
			// Loop through indices 0 and 1 to check the corresponding shapes and coordinates
			for (int i = 0; i <= 1; i++) {
				int k = i;
				// Check until reaching a relevant index (6 or 7)
				while (k <= i + 6) {
					// Check if the shape at the current index has a value of 1
					if (shapes[shape][k] == 1) {
						// Ensure that the next row (i + 1) is within the game area boundary
						if (coordinates[k].i + 1 < 20) { //bottom edge
							// Check if the position in the next row is occupied
							if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
								// Return 1 if the position is occupied (collision detected)
								return 1;
							}
						}
						else {
							return 1;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k += 2; // Skip to the next relevant index if no block at current index
				}
			}
			// If no collision is detected, return 0 (no collision)
			return 0;

			//return collision_down(0, 1);
		}

		else {	// rotation == 3, 270deg
			// Loop through indices 0, 2, 4 and 6 to check the corresponding shapes and coordinates
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				// Check until reaching a relevant index (0 or 1)
				while (k <= i + 1) {
					// Check if the shape at the current index has a value of 1
					if (shapes[shape][k] == 1) {
						// Ensure that the next row (i + 1) is within the game area boundary
						if (coordinates[k].i + 1 < 20) { //bottom edge
							// Check if the position in the next row is occupied
							if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
								// Return 1 if the position is occupied (collision detected)
								return 1;
							}
						}
						else {
							return 1;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					++k; // Skip to the next relevant index if no block at current index
				}
			}
			// If no collision is detected, return 0 (no collision)
			return 0;

			//return collision_down(0, 6);
		}
	}
}

//only tested for rotation == 0
bool brick::collision_down(int min, int max) {
	for (int i = min; i <= max; (rotation == 1 || rotation == 3)? i += 2 : ++i) {
		int k = i;
		// Check until reaching a relevant index 
		while ((rotation == 1 || rotation == 3) ? 
			k <= i + 1 && k >= i - 1 : //yes
			k <= i + 6 && k >= i - 6) { //no
			// Check if the shape at the current index has a value of 1
			if (shapes[shape][k] == 1) {
				// Ensure that the next row (i + 1) is within the game area boundary
				if (coordinates[k].i + 1 < 20) { //bottom edge
					// Check if the position in the next row is occupied
					if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
						// Return 1 if the position is occupied (collision detected)
						return 1;
					}
				}
				else {
					return 1;
				}
				// Exit the loop if a block is found and no collision detected
				break;
			}
			// Skip to the next relevant index if no block at current index
			switch (rotation) {
			case 0:
				k -= 2;
				break;
			case 1:
				--k;
				break;
			case 2:
				k += 2;
				break;
			case 3:
				++k;
				break;
			}
		}
	}
	// If no collision is detected, return 0 (no collision)
	return 0;
}
