#include "brick.h"


brick::brick(int col, int sh) : colour(col), shape(sh), rotation(0) {
	for (int i = 0; i < 8; i++) {
		coordinates[i].i = -1;
		coordinates[i].j = -1;
	}
}


void brick::draw(board* b) {
	for (int i = 0; i < 8; i++) {
		if (shapes[shape][i] != 0) {
			if (coordinates[i].i != -1 && coordinates[i].j != -1) {
				b->changeTile(coordinates[i].i, coordinates[i].j, this->colour);
				b->changeTileStatus(coordinates[i].i, coordinates[i].j, 1);
			}
		}
	}
}


