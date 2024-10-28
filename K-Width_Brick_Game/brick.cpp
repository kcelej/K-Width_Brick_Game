#include "brick.h"


brick::brick(int col, int sh, board* _b) : colour(col), shape(sh), rotation(0), b(_b) {
	// Initialize the initial position of each brick above the game area
	for (int k = 0; k < 8; ++k) {
		// Set coordinate i: -4 for the first two elements, -3 for the next two, and so on
		coordinates[k].i = -4 + k / 2;
		// Set coordinate j: 4 for even indices, 5 for odd indices
		coordinates[k].j = 4 + k % 2;
	}
}

void brick::draw() {
	for (int i = 0; i < 8; i++) {
		if (shapes[shape][i]) {
			if (coordinates[i].i >= 0 && coordinates[i].j >= 0) {
				b->changeTile(coordinates[i].i, coordinates[i].j, this->colour);
				b->changeTileStatus(coordinates[i].i, coordinates[i].j, 1);
			}
		}
	}
}

void brick::reset_entire_brick() {
	for (int i = 0; i < 8; i++) {
		if (shapes[shape][i]) b->resetTile(coordinates[i].i, coordinates[i].j);
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

//OLD VERSION
/*
bool brick::rotate() {
	//OLD VERSION
	
	//save the old coordinates temporarily 
	point Old_coordinates[8];
	for (int i = 0; i < 8; ++i) {
		Old_coordinates[i].copy_old_coordinates(&coordinates[i]);
	}
	//in the future, use switch to make it simpler and shorter
	if (rotation == 0) {
		//check for border
		if (coordinates[6].i - 3 < 0) {
			//can't rotate
			qDebug() << "Rotation not possible.";
			return 0;
		}

		//reset old tiles
		reset_entire_brick();

		//check for other blocks
		for (int i = 6; i >= 4; i -= 2) {
			for (int x = 0; x <= 3; ++x) {
				if (b->gameArea[coordinates[i].i - x][coordinates[i].j]->getIsOccupied()) {
					//can't rotate
					qDebug() << "Rotation not possible.";
					return 0;
				}
			}
		}
		
		//basic idea
		//from
		//[1][3][5][7]
		//[0][2][4][6]
		//to
				//[0][1]
				//[2][3]
				//[4][5]
				//[6][7]
		
		//rotate
		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd line then take y coordinates from 7, if even then y coordinates 6
			int y = Old_coordinates[6].i;
			//If we go odd line then take x coordinates from 6, if even then x coordinates 4
			int x = Old_coordinates[(i % 2) ? 6 : 4].j;
			while (k >= i - 6) {
				//x coordinates doesn't change only y
				coordinates[k].i = y--;
				coordinates[k].j = x;
				k -= 2;
			}
		}
		draw();
		// Rotation possible and done, return true
		qDebug() << "Rotation possible and done.";
		return 1;
	}
	else if (rotation == 1) {
		//check for border
		if (coordinates[6].j + 3 >= gameWindowWidth) {
			//can't rotate
			qDebug() << "Rotation not possible.";
			return 0;
		}
		//reset old tiles
		reset_entire_brick();
		//check for other blocks
		for (int i = 6; i >= 4; i -= 2) {
			for (int x = 0; x <= 3; ++x) {
				if (b->gameArea[coordinates[i].i][coordinates[i].j + x]->getIsOccupied()) {
					//can't rotate
					qDebug() << "Rotation not possible.";
					return 0;
				}
			}
		}
		
		//basic idea
		//from
		//[0][1]
		//[2][3]
		//[4][5]
		//[6][7]

		//to
				//[6][4][2][0]
				//[7][5][3][1]

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = Old_coordinates[(i % 2) ? 7 : 4].i;
			int x = Old_coordinates[6].j;
			while (k >= i - 6) {
				//y coordinates doesn't change only x
				coordinates[k].i = y;
				coordinates[k].j = x++;
				k -= 2;
			}
		}
		draw();
		// Rotation possible and done, return true
		qDebug() << "Rotation possible and done.";
		return 1;
	}

	if (rotation == 2) { // 180 degrees
		//check for border
		if (coordinates[6].i + 3 >= gameWindowHeight) {
			//can't rotate
			qDebug() << "Rotation not possible.";
			return 0;
		}

		//reset old tiles
		reset_entire_brick();

		//check for other blocks
		for (int i = 6; i >= 4; i -= 2) {
			for (int x = 0; x <= 3; ++x) {
				if (b->gameArea[coordinates[i].i + x][coordinates[i].j]->getIsOccupied()) {
					//can't rotate
					qDebug() << "Rotation not possible.";
					return 0;
				}
			}
		}

		//basic idea
		//from
		//[6][4][2][0]
		//[7][5][3][1]

		//to
				//[7][6]
				//[5][4]
				//[3][2]
				//[1][0]

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = Old_coordinates[6].i;
			int x = Old_coordinates[(i % 2) ? 6 : 4].j;
			while (k >= i - 6) {
				//x coordinates doesn't change only y
				coordinates[k].i = y++;
				coordinates[k].j = x;
				k -= 2;
			}
		}
		draw();
		// Rotation possible and done, return true
		qDebug() << "Rotation possible and done.";
		return 1;
	}

	else {	// rotation == 3
		//check for border
		if (coordinates[6].j - 3 < 0) {
			//can't rotate
			qDebug() << "Rotation not possible.";
			return 0;
		}

		//reset old tiles
		reset_entire_brick();

		//check for other blocks
		for (int i = 6; i >= 4; i -= 2) {
			for (int x = 0; x <= 3; ++x) {
				if (b->gameArea[coordinates[i].i][coordinates[i].j - x]->getIsOccupied()) {
					//can't rotate
					qDebug() << "Rotation not possible.";
					return 0;
				}
			}
		}

		//basic idea
		//from
		//[7][6]
		//[5][4]
		//[3][2]
		//[1][0]

		/to
				//[1][3][5][7]
				//[0][2][4][6]

		for (int i = 6; i <= 7; ++i) {
			int k = i;
			//If we go odd column then take y coordinates from 6, if even then x coordinates 4
			int y = Old_coordinates[(i % 2) ? 6 : 4].i;
				int x = Old_coordinates[6].j;
			while (k >= i - 6) {
				//y coordinates doesn't change only x
				coordinates[k].i = y;
				coordinates[k].j = x--;
				k -= 2;
			}
		}
		draw();
		// Rotation possible and done, return true
		qDebug() << "Rotation possible and done.";
		return 1;
	}
}
*/

//NEW VERSION
bool brick::rotate() {
	if (shape == 1) return false; //no need to rotate O
	// Save the old coordinates temporarily
	point Old_coordinates[8];
	for (int i = 0; i < 8; ++i) {
		Old_coordinates[i].copy_old_coordinates(&coordinates[i]);
	}

	// Check boundaries and reset tiles
	if (!checkBoundariesAndReset()) {
		return false; // Can't rotate
	}

	// Check for other blocks
	if (!checkForOtherBlocks()) {
		return false; // Can't rotate
	}

	// Perform rotation based on the current rotation value
	switch (rotation) {
	case 0:
		rotateDefault(Old_coordinates);
		break;
	case 1:
		rotate90Degrees(Old_coordinates);
		break;
	case 2:
		rotate180Degrees(Old_coordinates);
		break;
	case 3:
		rotate270Degrees(Old_coordinates);
		break;
	default:
		qDebug() << "Invalid rotation.";
		return false;
	}

	draw();
	qDebug() << "Rotation possible and done.";
	return true;
}

bool brick::checkBoundariesAndReset() {
	if ((rotation == 0 && coordinates[6].i - 3 < 0) ||
		(rotation == 1 && coordinates[6].j + 3 >= gameWindowWidth) ||
		(rotation == 2 && coordinates[6].i + 3 >= gameWindowHeight) ||
		(rotation == 3 && coordinates[6].j - 3 < 0)) {
		qDebug() << "Rotation not possible.";
		return false;
	}

	reset_entire_brick();
	return true;
}

bool brick::checkForOtherBlocks() {
	for (int i = 6; i >= 4; i -= 2) {
		for (int x = 0; x <= 3; ++x) {
			int newX = (rotation == 1 || rotation == 3) ? coordinates[i].j + (rotation == 1 ? x : -x) : coordinates[i].j;
			int newY = (rotation == 0 || rotation == 2) ? coordinates[i].i + (rotation == 2 ? x : -x) : coordinates[i].i;

			if (b->gameArea[newY][newX]->getIsOccupied()) {
				qDebug() << "Rotation not possible.";
				return false;
			}
		}
	}
	return true;
}

void brick::rotateDefault(point* Old_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = Old_coordinates[6].i;
		int x = Old_coordinates[(i % 2) ? 6 : 4].j;

		while (k >= i - 6) {
			coordinates[k].i = y--;
			coordinates[k].j = x;
			k -= 2;
		}
	}
}

void brick::rotate90Degrees(point* Old_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = Old_coordinates[(i % 2) ? 7 : 4].i;
		int x = Old_coordinates[6].j;

		while (k >= i - 6) {
			coordinates[k].i = y;
			coordinates[k].j = x++;
			k -= 2;
		}
	}
}

void brick::rotate180Degrees(point* Old_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = Old_coordinates[6].i;
		int x = Old_coordinates[(i % 2) ? 6 : 4].j;

		while (k >= i - 6) {
			coordinates[k].i = y++;
			coordinates[k].j = x;
			k -= 2;
		}
	}
}

void brick::rotate270Degrees(point* Old_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = Old_coordinates[(i % 2) ? 6 : 4].i;
		int x = Old_coordinates[6].j;

		while (k >= i - 6) {
			coordinates[k].i = y;
			coordinates[k].j = x--;
			k -= 2;
		}
	}
}

bool brick::collision(int direction) const {	// direction of falling: 0 - down, 1 - left, 2 - right
	if (direction == 1) {
		return 0;
	}
	else if (direction == 2) {
		return 0;
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
			/*
			[0][1]
			[2][3]
			[4][5]
			[6][7]
			*/
			return collision_down(6, 7);
		}

		else if (rotation == 1) { //90deg
			//delete in the future
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
			/*
			[6][4][2][0]
			[7][5][3][1]
			*/
			return collision_down(1, 7);
		}

		else if (rotation == 2) { // 180 degrees rotation
			//delete in the future
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
			/*
			[7][6]
			[5][4]
			[3][2]
			[1][0]
			*/
			return collision_down(0, 1);
		}
		else {	// rotation == 3, 270deg
			//delete in the future
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
			/*
			[1][3][5][7]
			[0][2][4][6]
			*/
			return collision_down(0, 6);
		}
	}
}

bool brick::collision_down(int min, int max) const {
	// Ensure that the next row (i + 1) is within the game area boundary
	if (coordinates[min].i + 1 >= 20) return 1;

	for (int i = min; i <= max; (rotation == 1 || rotation == 3)? i += 2 : ++i) {
		int k = i;
		// Check until reaching a relevant index 
		while ((rotation == 1 || rotation == 3) ? 
			k <= i + 1 && k >= i - 1 : //yes
			k <= i + 6 && k >= i - 6) { //no
			// Check if the shape at the current index has a value of 1
			if (shapes[shape][k] == 1) {
				// Check if the position in the next row is occupied
				if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied() == 1) {
					// Return 1 if the position is occupied (collision detected)
					qDebug() << "collision detected.";
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
			default:
				//exception
				break;
			}
		}
	}
	// If no collision is detected, return 0 (no collision)
	return 0;
}
