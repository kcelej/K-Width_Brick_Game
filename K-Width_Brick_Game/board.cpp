#include "board.h"


board::board(QGraphicsScene* scene, QGraphicsView* view) {
	int boardX = 28;
	int boardY = 30;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
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
	if (i >= 0 && i < 20 && j >= 0 && j < 10 && gameArea[i][j] != nullptr) {
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

void board::resetTile(int i, int j) {
	if (i >= 0 && i < 20 && j >= 0 && j < 10 && gameArea[i][j] != nullptr) {
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