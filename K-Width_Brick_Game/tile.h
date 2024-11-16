#pragma once

#include <QPixMap>
#include <QGraphicsPixmapItem>

const int tileSize = 18;	// tile size in pixels (one side of a square)

class tile
{
private:
	QGraphicsPixmapItem* ptr;	// pointer to the tile QGraphicsItem object
	bool isOccupied;	// is the tile occupied (0 - no, 1 - yes)
	int colour;	// the colour of the tile (values: 0-7)
public:
	tile();
	tile(int x, int y);

	QGraphicsPixmapItem* getPtr();
	bool getIsOccupied();
	void changeTileStatus(bool b);

	tile& copy_values(const tile* old) {
		this->ptr = old->ptr;
		this->isOccupied = old->isOccupied;	
		this->colour = old->colour;
		return *this;
	}
};

