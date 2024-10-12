#pragma once
#include <QGraphicsItemGroup>

class brick : public QGraphicsItemGroup
{
public:
	int tile_size;
	const int shapes[7][4] = {
        1,3,5,7, // I
        2,4,5,7, // Z
        3,5,4,6, // S
        3,5,4,7, // T
        2,3,5,7, // L
        3,5,7,6, // J
        2,3,4,5, // O
    };

    struct point {
        int x, y;
    } coordinates[4];

	brick(int t_s);

	void draw();
};

