#pragma once

#include <QPixMap>
#include <QGraphicsPixmapItem>

const int tileSize = 18;	// tile size in pixels (one side of a square)

class tile
{
private:
	QGraphicsPixmapItem* ptr;	// pointer to the tile QGraphicsItem object
	bool isOccupied;	// is the tile occupied (0 - no, 1 - yes)
	int color;	// the colour of the tile (values: 0-7)
public:
	tile();
	tile(int x, int y);

	QGraphicsPixmapItem* getPtr();
	bool getIsOccupied();
	void changeTileStatus(bool b);
	int get_color() { return color; }
};

