#include "brick.h"


brick::brick(int col, int sh) : colour(col), shape(sh), rotation(0) {
	for (int i = 0; i < 8; i++) {
		coordinates[i].i = -1;
		coordinates[i].j = -1;
	}
}


void brick::draw(board* b) {
	if (rotation == 0) {
		for (int i = 0; i < 8; i++) {
			if (shapes[shape][i] != 0) {
				if (coordinates[i].i != -1 && coordinates[i].j != -1) {
					b->changeTile(coordinates[i].i, coordinates[i].j, this->colour);
					b->changeTileStatus(coordinates[i].i, coordinates[i].j, 1);
				}
			}
		}
	}
	else if (rotation == 1) {
		//basic idea
		/*for (int i = 0; i < 8; i++) {
			int nowy_x = coordinates[7].j - (coordinates[i].i - coordinates[7].i);
			int nowy_y = coordinates[7].i + (coordinates[i].j - coordinates[7].j);

			b->changeTile(nowy_y, nowy_x, this->colour);
			b->changeTileStatus(nowy_y, nowy_x, 1);
		}*/
	}
	if (rotation == 2) { // 180 degrees

	}
	else {	// rotation == 3

	}
}


bool brick::collision(board* b, int direction) {	// direction of falling: 0 - down, 1 - left, 2 - right
	if (direction == 1) {

	}
	else if (direction == 2) {

	}
	else {	// direction == 0
		if (rotation == 0) {
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
			return collision_down(b, 0, 6, 7);
		}

		else if (rotation == 1) { //90deg
			//// Loop through indices 1, 3, 5 and 7 to check the corresponding shapes and coordinates
			//for (int i = 1; i <= 7; i += 2) {
			//	int k = i;
			//	// Check until reaching a relevant index (0 or 1)
			//	while (k >= i - 1) {
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
			//		--k; // Skip to the next relevant index if no block at current index
			//	}
			//}
			//// If no collision is detected, return 0 (no collision)
			//return 0;

			return collision_down(b, 1, 1, 7);
		}

		else if (rotation == 2) { // 180 degrees rotation
			if (rotation == 0) {
				//// Loop through indices 0 and 1 to check the corresponding shapes and coordinates
				//for (int i = 0; i <= 1; i++) {
				//	int k = i;
				//	// Check until reaching a relevant index (6 or 7)
				//	while (k <= i + 6) {
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
				//		k += 2; // Skip to the next relevant index if no block at current index
				//	}
				//}
				//// If no collision is detected, return 0 (no collision)
				//return 0;

				return collision_down(b, 2, 0, 1);
			}
		}

		else {	// rotation == 3, 270deg
			//// Loop through indices 0, 2, 4 and 6 to check the corresponding shapes and coordinates
			//for (int i = 0; i <= 6; i += 2) {
			//	int k = i;
			//	// Check until reaching a relevant index (0 or 1)
			//	while (k <= i + 1) {
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
			//		++k; // Skip to the next relevant index if no block at current index
			//	}
			//}
			//// If no collision is detected, return 0 (no collision)
			//return 0;

			return collision_down(b, 3, 0, 6);
		}
	}
}

bool brick::collision_down(board* b, int rotation, int min, int max) {
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
