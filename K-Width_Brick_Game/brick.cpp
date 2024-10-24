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

	}
	else if (rotation == 2) {
		
	}
	else {	// rotation == 3

	}
}


bool brick::collision(board* b, int direction) {	// directions: 0 - down, 1 - left, 2 - right
	if (direction == 1) {

	}
	else if (direction == 2) {

	}
	else {	// direction == 0
		if (rotation == 0) {
			if (shapes[shape][6] == 1 || shapes[shape][7] == 1) {
				if (coordinates[6].i + 1 < 20 && coordinates[7].i + 1 < 20) {
					if ((b->gameArea[coordinates[6].i + 1][coordinates[6].j]->getIsOccupied() == 1) && (b->gameArea[coordinates[7].i + 1][coordinates[7].j]->getIsOccupied() == 1)) {
						return 1;
					}
					else {
						return 0;
					}
				}
				else {
					return 1;
				}
			}
		}	
	}
}
