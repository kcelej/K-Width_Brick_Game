#include "brick.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QRect>

brick::brick(int t_s) : tile_size(t_s) {
	draw();
}

void brick::draw() {
	QPixmap tileset(":/resources/textures/tiles.png");
	int color = 2;

	QRect rectangle(2*tile_size, 0, tile_size, tile_size);
	QPixmap cutTexture;

	cutTexture = tileset.copy(rectangle);

	for (int i = 0; i < 4; i++) {
		coordinates[i].x = shapes[color][i] % 2;
		coordinates[i].y = shapes[color][i] / 2;
	}

	for (int i = 0; i < 4; i++) {
		QGraphicsPixmapItem* square = new QGraphicsPixmapItem();

		square->setPixmap(cutTexture);

		square->setPos(coordinates[i].x*tile_size, coordinates[i].y*tile_size);

		this->addToGroup(square);
	}
	

}