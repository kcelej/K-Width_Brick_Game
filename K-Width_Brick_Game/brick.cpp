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
		// Check if the current part is active in the current shape
		if (shapes[shape][i]) {
			// Verify that the coordinates are within valid range
			if (coordinates[i].i >= 0 && coordinates[i].j >= 0) {
				// Set the tile to the brick's color
				b->changeTile(coordinates[i].i, coordinates[i].j, this->colour);
				// Mark the tile as occupied
				b->changeTileStatus(coordinates[i].i, coordinates[i].j, 1);
			}
		}
	}
}

void brick::reset_entire_brick() {
	for (int i = 0; i < 8; i++) {
		// Check if the current part has valid coordinates
		if (coordinates[i].i >= 0 && coordinates[i].j >= 0) {
			// If the part exists in the current shape, reset its tile in the game area
			if (shapes[shape][i]) b->resetTile(coordinates[i].i, coordinates[i].j);
		}
	}
}

bool brick::move_down() {
	// Check for collision with the border or other blocks below
	if (!collision(0)) {
		// No collision detected, move each part of the brick down by one block
		for (int i = 0; i < 8; i++) {
			coordinates[i].i++;
		}
		// Brick successfully moved down
		return true;
	}
	// Collision detected; stop moving the brick down
	qDebug() << "Stop moving this block.";
	return false;
}

void brick::move_left() {
	// Check for collision with the border or other blocks to the left
	if (!collision(1)) {
		// No collision detected, move each part of the brick one block to the left
		for (int i = 0; i < 8; i++) {
			coordinates[i].j--;
		}
		qDebug() << "Moved to the left.";
	}
	// If collision is detected, the brick remains in its current position
}

void brick::move_right() {
	// Check for collision with the border or other blocks to the right
	if (!collision(2)) {
		// No collision detected, move each part of the brick one block to the right
		for (int i = 0; i < 8; i++) {
			coordinates[i].j++;
		}
		qDebug() << "Moved to the right.";
	}
	// If collision is detected, the brick remains in its current position
}

bool brick::movement(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Up:
		qDebug() << "Up arrow pressed";
		change_rotation();
		return true;
		break;

	case Qt::Key_Down:
		qDebug() << "Down arrow pressed";
		move_down();
		return true;
		break;

	case Qt::Key_Left:
		qDebug() << "Left arrow pressed";
		move_left();
		return true;
		break;

	case Qt::Key_Right:
		qDebug() << "Right arrow pressed";
		move_right();
		return true;
		break;

	default:
		qDebug() << "Other key pressed";
		return false;
		break;
	}
	return false;
}

void brick::change_rotation() {
	int old_rotation = rotation;
	increment_rotation();
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
	point Tmp_coordinates[8];
	for (int i = 0; i < 8; ++i) {
		Tmp_coordinates[i].copy_coordinates(&coordinates[i]);
	}

	reset_entire_brick();

	// Perform rotation based on the current rotation value
	switch (rotation) {
	case 0:
		rotateDefault(Tmp_coordinates);
		break;
	case 1:
		rotate90Degrees(Tmp_coordinates);
		break;
	case 2:
		rotate180Degrees(Tmp_coordinates);
		break;
	case 3:
		rotate270Degrees(Tmp_coordinates);
		break;
	default:
		qDebug() << "Invalid rotation.";
		return false;
	}

	reset_entire_brick();

	// Collision check for temporary coordinates
	if (checkForCollision_Rotation(Tmp_coordinates)) {
		qDebug() << "Rotation not possible due to collision.";
		return false;
	}

	// If there are no collisions assign new positions 
	for (int i = 0; i < 8; ++i) {
		coordinates[i].copy_coordinates(&Tmp_coordinates[i]);
	}

	draw();
	qDebug() << "Rotation possible and done.";
	return true;
}

bool brick::checkForCollision_Rotation(const point* temp_coords) const {
	for (int i = 0; i < 8; i++) {
		// Check if the current brick part is part of the shape and has visible coordinates
		// If the coordinates are out of the game window or occupied by another block, it indicates a collision
		if (shapes[shape][i] && temp_coords[i].i >= 0 && temp_coords[i].j >= 0 &&
			(temp_coords[i].i >= gameWindowHeight || temp_coords[i].j >= gameWindowWidth ||
				b->gameArea[temp_coords[i].i][temp_coords[i].j]->getIsOccupied())) {
			// Collision detected
			return true;
		}
	}
	// No collision detected
	return false;
}

void brick::rotateDefault(point* Temp_coordinates) {
    for (int i = 6; i <= 7; ++i) {
        int k = i;
        int y = coordinates[6].i;
        int x = coordinates[(i % 2) ? 6 : 4].j;

        while (k >= i - 6) {
            Temp_coordinates[k].i = y--;
            Temp_coordinates[k].j = x;
            k -= 2;
        }
    }
}

void brick::rotate90Degrees(point* Temp_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = coordinates[(i % 2) ? 7 : 4].i;
		int x = coordinates[6].j;

		while (k >= i - 6) {
			Temp_coordinates[k].i = y;
			Temp_coordinates[k].j = x++;
			k -= 2;
		}
	}
}

void brick::rotate180Degrees(point* Temp_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = coordinates[6].i;
		int x = coordinates[(i % 2) ? 6 : 4].j;

		while (k >= i - 6) {
			Temp_coordinates[k].i = y++;
			Temp_coordinates[k].j = x;
			k -= 2;
		}
	}
}

void brick::rotate270Degrees(point* Old_coordinates) {
	for (int i = 6; i <= 7; ++i) {
		int k = i;
		int y = coordinates[(i % 2) ? 6 : 4].i;
		int x = coordinates[6].j;

		while (k >= i - 6) {
			Old_coordinates[k].i = y;
			Old_coordinates[k].j = x--;
			k -= 2;
		}
	}
}

//NEW VERSION BUT LAME
//Delete if the new method is 100% correct
/*
bool brick::collision(int direction) {
	if (direction == 0) {
		if (rotation == 0) {
			//check for border
			for (int i = 7; i >= 0; i--) {
				if (shape[shapes][i] && coordinates[i].i >= 0) {
					if (coordinates[i].i + 1 >= Game_Area_Height) {
						qDebug() << "Collision with border.";
						return true;
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
			}
			//check for collision with other bricks
			for (int i = 6; i <= 7; i++) {
				int k = i;
				while (k >= i - 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the next row is occupied
						if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "Collision with another brick.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k -= 2; // Skip to the next relevant index if no block at current index
				}
			}
		}
		else if (rotation == 1) { //90deg
			//check for border
			for (int i = 1; i <= 7; i += 2) {
				int k = i;
				while (k > i - 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].i + 1 >= Game_Area_Height) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k--;
				}
			}

			//check for collision with other bricks
			for (int i = 1; i <= 7; i += 2) {
				int k = i;
				while (k >= i - 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the next row is occupied
						if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "Collision with another brick.";
							return 1;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					--k; // Skip to the next relevant index if no block at current index
				}
			}
		}
		else if (rotation == 2) { //180deg
			//check for border
			for (int i = 0; i <= 7; i++) {
				if (shape[shapes][i] && coordinates[i].i >= 0) {
					if (coordinates[i].i + 1 >= Game_Area_Height) {
						qDebug() << "Collision with border.";
						return true;
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 1; i++) {
				int k = i;
				while (k <= i + 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the next row is occupied
						if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							return 1;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k += 2; // Skip to the next relevant index if no block at current index
				}
			}
		}
		else if (rotation == 3) { //270deg
			//check for border
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k < i + 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].i + 1 >= Game_Area_Height) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k++;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k <= i + 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the next row is occupied
						if (b->gameArea[coordinates[k].i + 1][coordinates[k].j]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					++k; // Skip to the next relevant index if no block at current index
				}
			}
		}
	}	
	else if (direction == 1) {
		if (rotation == 0) { //default
			//check for border
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k <= i + 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j - 1 < 0) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k++;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k <= i + 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j - 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k++; // Skip to the next relevant index if no block at current index
			}
		}
		else if (rotation == 1) { //90deg
			//check for border
			for (int i = 6; i <= 7; i++) {
				int k = i;
				while (k >= i - 6) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j - 1 < 0) {
							qDebug() << "Collision with border.";
							return 1;
						}
						break;
					}
					k -= 2;
				}
			}
			//check for collision with other bricks
			for (int i = 6; i <= 7; i++) {
				int k = i;
				while (k >= i - 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j - 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k -= 2; // Skip to the next relevant index if no block at current index
				}
			}
		}
		else if (rotation == 2) { //180deg
			//check for border
			for (int i = 1; i <= 7; i++) {
				int k = i;
				while (k >= i - 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j - 1 < 0) {
							qDebug() << "Collision with border.";
							return true;
						}
						break;
					}
					k--;
				}
			}
			//check for collision with other bricks
			for (int i = 1; i <= 7; i++) {
				int k = i;
				while (k >= i - 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j - 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k--; // Skip to the next relevant index if no block at current index
			}
		}
		else if (rotation == 3) { //270deg
			//check for border
			for (int i = 0; i <= 1; i++) {
				int k = i;
				while (k <= i + 6) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j - 1 < 0) {
							qDebug() << "Collision with border.";
							return true;
						}
						break;
					}
					k += 2;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 1; i++) {
				int k = i;
				while (k <= i + 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j - 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k += 2; // Skip to the next relevant index if no block at current index
			}
		}
	}
	else if (direction == 2) { // right
		if (rotation == 0) { //default
			//check for border
			for (int i = 1; i <= 7; i += 2) {
				int k = i;
				while (k >= i - 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j + 1 >= Game_Area_Width) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k--;
				}
			}
			//check for collision with other bricks
			for (int i = 1; i <= 7; i += 2) {
				int k = i;
				while (k >= i - 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j + 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k--;; // Skip to the next relevant index if no block at current index
			}
		}
		else if (rotation == 1) { //90deg
			//check for border
			for (int i = 0; i <= 1; i++) {
				int k = i;
				while (k <= i + 6) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j + 1 >= Game_Area_Width) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k += 2;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 1; i++) {
				int k = i;
				while (k <= i + 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j + 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k += 2; // Skip to the next relevant index if no block at current index
				}
			}
		}
		else if (rotation == 2) { //180deg
			//check for border
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k <= i + 1) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j + 1 >= Game_Area_Width) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k++;
				}
			}
			//check for collision with other bricks
			for (int i = 0; i <= 6; i += 2) {
				int k = i;
				while (k <= i + 1) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j + 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k++; // Skip to the next relevant index if no block at current index
			}
		}
		else if (rotation == 3) { //270deg
			//check for border
			for (int i = 6; i <= 7; i++) {
				int k = i;
				while (k >= i - 6) {
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						if (coordinates[k].j + 1 >= Game_Area_Width) {
							qDebug() << "Collision with border.";
							return true;
						}
						// Exit the loop if a block is found and no collision detected
						break;
					}
					k -= 2;
				}
			}
			//check for collision with other bricks
			for (int i = 6; i <= 7; i++) {
				int k = i;
				while (k >= i - 6) {
					// Check if the shape at the current index has a value of 1
					if (shape[shapes][k] && coordinates[k].i >= 0) {
						// Check if the position in the previous collumn is occupied
						if (b->gameArea[coordinates[k].i][coordinates[k].j + 1]->getIsOccupied()) {
							// Return 1 if the position is occupied (collision detected)
							qDebug() << "collision with another block detected.";
							return true;
						}
					}
					// Exit the loop if a block is found and no collision detected
					break;
				}
				k -= 2; // Skip to the next relevant index if no block at current index
			}
		}
	}
	//no collision detected
	return 0;
}
*/

//NEW BETTER VERSION
//Generally tested, it is not known whether there are any special exceptions
bool brick::checkForCollision_Movement(int index, int offset_i, int offset_j) const {
	// Calculate the target coordinates for the brick part after applying the offset
	int target_i = coordinates[index].i + offset_i;
	int target_j = coordinates[index].j + offset_j;

	// Check if the target coordinates are outside the game area boundaries
	if (target_i < 0 || target_i >= Game_Area_Height || target_j < 0 || target_j >= Game_Area_Width) {
		qDebug() << "Collision with border.";
		return true;
	}

	// Check if the target position is occupied by another block
	if (b->gameArea[target_i][target_j]->getIsOccupied()) {
		qDebug() << "Collision with another block.";
		return true;
	}

	// No collision detected, return false
	return false;
}

//NEW BETTER VERSION
//Generally tested, it is not known whether there are any special exceptions
bool brick::collision(int direction) {
	int offset_i = 0, offset_j = 0;

	switch (direction) {
	case 0: offset_i = 1; break; // Down
	case 1: offset_j = -1; break; // Left
	case 2: offset_j = 1; break; // Right
	default: return false;
	}

	// Check collision for each part of the brick
	for (int i = 0; i < 8; i++) {
		if (shape[shapes][i] && coordinates[i].i >= 0) {
			if (checkForCollision_Movement(i, offset_i, offset_j)) {
				return true; // Collision detected
			}
		}
	}

	// No collision detected
	return false;
}

void brick::increment_rotation() {
	if (rotation + 1 < 4) {
		rotation++;
	}
	else {
		rotation = 0;
	}
}
