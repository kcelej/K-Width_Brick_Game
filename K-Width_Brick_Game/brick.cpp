#pragma once
#include "brick.h"

brick::brick(int col, int sh, board* _b) : color(col), shape(sh), rotation(0), b(_b) {
	// Initialize the initial position of each brick above the game area
	for (int tile = 0; tile < ALL_TILES; ++tile) {
		// Set row coordinate: -4 for the first two tiles, -3 for the next two, etc.
		coordinates[tile].i = -4 + tile / 2;
		// Set coordinate j: 4 for even indices, 5 for odd indices
		coordinates[tile].j = 4 + tile % 2;
	}
}

void brick::draw() const {
	for (int tile = 0; tile < ALL_TILES; tile++) {
		// Check if the current part is active in the current shape
		if (get_tile_presence(tile)) {
			// Verify that the coordinates are within valid range
			if (coordinates[tile].i >= 0 && coordinates[tile].j >= 0) {
				// Set the tile color to match the brick.
				b->changeTile(coordinates[tile].i, coordinates[tile].j, color);
				// Mark the tile as occupied
				b->changeTileStatus(coordinates[tile].i, coordinates[tile].j, true);
			}
		}
	}
}

void brick::reset_entire_brick() const{
	for (int tile = 0; tile < ALL_TILES; tile++) {
		// Check if the current part is visible
		if (coordinates[tile].i >= 0) {
			// If the part exists in the current shape, reset its tile in the game area
			if (get_tile_presence(tile)) b->resetTile(coordinates[tile].i, coordinates[tile].j);
		}
	}
}

void brick::movement(QKeyEvent* event) {
	switch (event->key()) {
	case Qt::Key_Up: case Qt::Key_W: // Rotate the brick.
		change_rotation();
		break;

	case Qt::Key_Down: case Qt::Key_S: // Move the brick down.
		reset_entire_brick();
		can_be_still_moved = move_down();
		break;

	case Qt::Key_Left: case Qt::Key_A: // Move the brick left.
		move_left();
		break;

	case Qt::Key_Right: case Qt::Key_D: // Move the brick right.
		move_right();
		break;

	default:
		break;
	}
	// Redraw the brick after movement.
	draw();
}

void brick::move_down_by_game() {
	reset_entire_brick();
	can_be_still_moved = move_down();
	draw();
}

void brick::change_rotation() {
	int old_rotation = rotation;
	increment_rotation();
	reset_entire_brick();

	// If rotation does not cause any collisions, apply it.
	if (rotate()) return;

	// If rotation is not possible, revert to the old rotation state.
	rotation = old_rotation;
}

bool brick::rotate() {
	if (shape == 1) return false; // O-shape does not need rotation.

	// Save the current coordinates temporarily.
	point Tmp_coordinates[ALL_TILES];
	for (int tile = 0; tile < ALL_TILES; ++tile) {
		Tmp_coordinates[tile].copy_coordinates(coordinates[tile]);
	}

	// Perform rotation based on the current state.
	switch (rotation) {
	case 0: rotateDefault(Tmp_coordinates); break;
	case 1: rotate90Degrees(Tmp_coordinates); break;
	case 2: rotate180Degrees(Tmp_coordinates); break;
	case 3: rotate270Degrees(Tmp_coordinates); break;
	default: return false;
	}

	// Check for collisions.
	if (checkForCollision_Rotation(Tmp_coordinates)) {
		// Rotation not possible due to collision.
		return false;
	}

	// Apply the new rotation.
	for (int tile = 0; tile < ALL_TILES; ++tile) {
		coordinates[tile].copy_coordinates(Tmp_coordinates[tile]);
	}

	return true;
}

bool brick::checkForCollision_Rotation(const point* temp_coords) const {
	for (int tile = 0; tile < ALL_TILES; tile++) {
		if (get_tile_presence(tile) && temp_coords[tile].i >= 0 && (temp_coords[tile].i >= GAME_AREA_HEIGHT || temp_coords[tile].j >= GAME_AREA_WIDTH || 
			temp_coords[tile].j < 0 || b->gameArea[temp_coords[tile].i][temp_coords[tile].j]->getIsOccupied())) {
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

bool brick::checkForCollision_Movement_Brick(int index, int offset_i, int offset_j) const {
	// Calculate the target coordinates after applying the movement offset.
	int target_i = coordinates[index].i + offset_i;
	int target_j = coordinates[index].j + offset_j;

	// Ignore negative row values (outside visible area).
	if (target_i < 0) { return false; }

	// Check if the target position is occupied by another brick.
	if (b->gameArea[target_i][target_j]->getIsOccupied()) {
		// Collision with another brick detected.
		return true;
	}
	// No collision detected
	return false;
}

bool brick::checkForCollision_Movement_Border(int index, int offset_i, int offset_j) const {
	// Calculate the target coordinates after applying the movement offset.
	int target_i = coordinates[index].i + offset_i;
	int target_j = coordinates[index].j + offset_j;

	// Check if the target position is outside the game area boundaries.
	if (target_i >= GAME_AREA_HEIGHT || target_j < 0 || target_j >= GAME_AREA_WIDTH) {
		// Collision with the border detected.
		return true;
	}
	// No collision detected
	return false;
}

bool brick::collision(int direction) const{
	int offset_i = 0, offset_j = 0;

	// Determine movement direction based on input.
	switch (direction) {
	case 0: offset_i = 1; break; // Down
	case 1: offset_j = -1; break; // Left
	case 2: offset_j = 1; break; // Right
	default: return false; // Invalid direction, return no collision.
	}

	// Check for collision with the border or other bricks.
	for (int tile = 0; tile < ALL_TILES; tile++) {
		if (get_tile_presence(tile)) {
			if (checkForCollision_Movement_Border(tile, offset_i, offset_j)) {
				return true; // Collision detected with the border.
			}
			if (checkForCollision_Movement_Brick(tile, offset_i, offset_j)) {
				return true; // Collision detected with another brick.
			}
		}
	}
	// No collision detected
	return false;
}

bool brick::move_down() {
	reset_entire_brick();
	if (!collision(DIRECTION_DOWN)) {
		for (int tile = 0; tile < ALL_TILES; tile++) {
			// Move the brick one row down.
			coordinates[tile].i++;
		}
		// Movement successful.
		return true;
	}
	// Collision detected, brick cannot move.
	return false;
}

void brick::move_left() {
	reset_entire_brick();
	if (!collision(DIRECTION_LEFT)) {
		for (int tile = 0; tile < ALL_TILES; tile++) {
			// Move the brick one column left.
			coordinates[tile].j--;
		}
	}
}

void brick::move_right() {
	reset_entire_brick();
	if (!collision(DIRECTION_RIGHT)) {
		for (int tile = 0; tile < ALL_TILES; tile++) {
			// Move the brick one column right.
			coordinates[tile].j++;
		}
	}
}

bool brick::chceck_for_defeat() {
	for (int tile = 0; tile < ALL_TILES; tile++) {
		if (coordinates[tile].i < 0) {
			// A part of the brick is above the board, indicating game over.
			return true;
		}
	}
	// No game-over condition detected.
	return false;
}

void brick::increment_rotation() {
	(rotation + 1 < 4) ? rotation++ : rotation = 0;
}