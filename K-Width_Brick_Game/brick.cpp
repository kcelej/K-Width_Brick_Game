#include "brick.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QRect>

brick::brick(int t_s, int c) : tile_size(t_s), color(c) {
	draw();

	this->setFlag(QGraphicsItem::ItemIsFocusable, 1);
	this->setFocus();
}

void brick::draw() {
	QPixmap tileset(":/resources/textures/tiles.png");

	QRect rectangle(color*tile_size, 0, tile_size, tile_size);
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

		if (i == 1) {
			this->setTransformOriginPoint(coordinates[i].x * tile_size, coordinates[i].y * tile_size);
		}
		
	}

	//this->setRotation(90);

}


void brick::keyPressEvent(QKeyEvent* k) {
	switch (k->key()) {
		case Qt::Key_Left:
			// move the brick left by one tile
			
			setX(x() - tile_size);
			break;
		case Qt::Key_Right:
			// move the brick right by one tile
			setX(x() + tile_size);
			break;
		//case Qt::Key_Down:
		//	// move the brick down by one tile
		//	// tbd
		//	break;
		//case Qt::Key_Up:
		//	// instantly move brick to the bottom
		//	// tbd
		//	break;
	}
}