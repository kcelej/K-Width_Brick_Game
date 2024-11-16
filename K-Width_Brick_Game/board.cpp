#include "board.h"


board::board(QGraphicsScene* scene, QGraphicsView* view) {
	int boardX = 28;
	int boardY = 30;
	for (int i = 0; i < Game_Area_Height; i++) {
		for (int j = 0; j < Game_Area_Width; j++) {
			tile* activeTile = new tile(boardX, boardY);
			boardX += tileSize;

			gameArea[i][j] = activeTile;
			scene->addItem(activeTile->getPtr());
		}
		boardX = 28;
		boardY += tileSize;
	}
}


void board::changeTile(int i, int j, int colour) {
	if (i >= 0 && i < Game_Area_Height && j >= 0 && j < Game_Area_Width && gameArea[i][j] != nullptr) {
		QPixmap tileset(":/resources/textures/tiles.png");
		if (!tileset.isNull()) {
			QRect rectangle(colour * tileSize, 0, tileSize, tileSize);
			QPixmap cutTexture = tileset.copy(rectangle);
			gameArea[i][j]->getPtr()->setPixmap(cutTexture);
		}
		else {
			qWarning("Failed to load the tileset in changeTile.");
		}
	}
	else {
		qWarning("Invalid access to gameArea[%d][%d] in changeTile", i, j);
	}
}


bool board::isTileTaken(int i, int j) {
	return gameArea[i][j]->getIsOccupied();
}

void board::changeTileStatus(int i, int j, bool b) {
	gameArea[i][j]->changeTileStatus(b);
}

//fix needed?
void board::resetTile(int i, int j) {
	if (i >= 0 && i < Game_Area_Height && j >= 0 && j < Game_Area_Width && gameArea[i][j] != nullptr) {
		QPixmap tileset(":/resources/textures/empty_tile.png");
		if (!tileset.isNull()) {
			QRect rectangle(tileSize, 0, tileSize, tileSize);
			QPixmap cutTexture = tileset.copy(rectangle);
			gameArea[i][j]->getPtr()->setPixmap(cutTexture);
			changeTileStatus(i, j, false);
		}
		else {
			qWarning("Failed to load the empty tile texture in resetTile.");
		}
	}
	else {
		qWarning("Invalid access to gameArea[%d][%d] in resetTile", i, j);
	}
}

bool board::check_line_condition(int i, bool check_for_occupied) {
	qDebug() << "Checking the line with condition:" << (check_for_occupied ? "Occupied" : "Empty");
	for (int j = 0; j < Game_Area_Width; j++) {
		bool is_occupied = gameArea[i][j]->getIsOccupied();
		if (check_for_occupied && !is_occupied) {
			qDebug() << "The line contains blanks.";
			return false; // The line is not fully occupied.
		}
		if (!check_for_occupied && is_occupied) {
			qDebug() << "The line contains bricks.";
			return false; // The line is not fully empty.
		}
	}
	qDebug() << (check_for_occupied ? "Full line occupied." : "Full line empty.");
	return true;
}

void board::delete_line(int i) {
	qDebug() << "Line removal.";
	for (int j = 0; j < Game_Area_Width; j++) {
		resetTile(i, j);
	}
}

void board::move_all_down(int i) {
	qDebug() << "Moving the lines.";
	for (int row = i; row >= 0; row--) {
		// If the row above does not exist (topmost row) or the row above is fully empty, delete the current row.
		if (row - 1 < 0 || check_line_condition(row - 1, false)) delete_line(row);
		else {
			for (int j = 0; j < Game_Area_Width; j++) {
				if (gameArea[row - 1][j]->getIsOccupied()) {
					changeTileStatus(row, j, true);
					changeTile(row, j, gameArea[row - 1][j]->get_color());
				}
			}
		}
	}
}

void board::check_board() {
	qDebug() << "Checking the board.";
	// Start checking from the bottom-most row and move upward.
	for (int i = Game_Area_Height - 1; i >= 0; i--) {
		// Continue checking the same row while it is completely filled.
		while (check_line_condition(i, true)) {
			// The line is full and can be removed.
			delete_line(i);
			// Shift all lines above the deleted line one row down.
			move_all_down(i);
		}
	}
}
