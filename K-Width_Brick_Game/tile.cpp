#include "tile.h"

tile::tile() : ptr(nullptr), isOccupied(0), colour(0) {}


tile::tile(int x, int y) : ptr(nullptr), isOccupied(0), colour(3) {

	QPixmap tileset(":/resources/textures/empty_tile.png");
	if (!tileset.isNull()) {
		QRect rectangle(tileSize, 0, tileSize, tileSize);
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
		this->isOccupied = b;
	}
	else {
		qWarning("Attempting to modify a null tile.");
	}
}