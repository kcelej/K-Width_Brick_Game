#include "tile.h"

tile::tile() : ptr(nullptr), isOccupied(0), color(0) {}


tile::tile(int x, int y) : ptr(nullptr), isOccupied(0), color(3) {

	QPixmap tileset(":/resources/textures/empty_tile.png");
	if (!tileset.isNull()) {
		QRect rectangle(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
		QPixmap cutTexture = tileset.copy(rectangle);
		ptr = new QGraphicsPixmapItem();

		ptr->setPixmap(cutTexture);
		ptr->setPos(x, y);
	}
	else {
		// Handle the case when the pixmap is not loaded
		qWarning("Failed to load the texture for the tile.");
	}
}


QGraphicsPixmapItem* tile::getPtr() {
	if (!ptr) {
		qWarning("QGraphicsPixmapItem is not initialized!");
	}
	return ptr;
}

bool tile::getIsOccupied() {
	return isOccupied;
}

void tile::changeTileStatus(bool b) {
	if (this->ptr != nullptr) {
		isOccupied = b;
	}
	else {
		qWarning("Attempting to modify a null tile.");
	}
}