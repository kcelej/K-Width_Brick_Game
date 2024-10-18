#include "tile.h"

tile::tile() : ptr(nullptr), isOccupied(0), colour(0) {}


tile::tile(int x, int y) : ptr(nullptr), isOccupied(0), colour(3) {

	QPixmap tileset("");
	QRect rectangle(colour * tileSize, 0, tileSize, tileSize);
	QPixmap cutTexture = tileset.copy(rectangle);
	ptr = new QGraphicsPixmapItem();

	ptr->setPixmap(cutTexture);
	ptr->setPos(x, y);
}


QGraphicsPixmapItem* tile::getPtr() {
	return ptr;
}