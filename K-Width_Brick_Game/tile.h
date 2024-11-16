#pragma once
#include <QPixMap>
#include <QGraphicsPixmapItem>
#include "Defines.h"


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
	void set_color(int c) {
		if (c >= 0 && c <= 7) color = c;
	}
};

