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
    QRect rectangle(color * tile_size, 0, tile_size, tile_size);
    QPixmap cutTexture = tileset.copy(rectangle);

    int sum_x = 0, sum_y = 0;  // To calculate the average position of all tiles

    for (int i = 0; i < 4; i++) {
        coordinates[i].x = shapes[color][i] % 2;
        coordinates[i].y = shapes[color][i] / 2;

        sum_x += coordinates[i].x;
        sum_y += coordinates[i].y;
    }

    // Calculate center based on the average of tile positions
    QPointF center((sum_x / 4.0) * tile_size, (sum_y / 4.0) * tile_size);

    for (int i = 0; i < 4; i++) {
        QGraphicsPixmapItem* square = new QGraphicsPixmapItem();
        square->setPixmap(cutTexture);
        square->setPos(coordinates[i].x * tile_size, coordinates[i].y * tile_size);

        this->addToGroup(square);
    }

    // Set the transform origin to the calculated center
    this->setTransformOriginPoint(center);
}



void brick::adjustAfterRotation() {
    QRectF boundingRect = this->boundingRect();
    
    if (x() < 27) {
        setX(27);  // Ensure it doesn’t go left out of bounds from the left side
    }
    if (x() + boundingRect.width() > 207) {
        setX(207 - boundingRect.width());  // Ensure it doesn’t go out of bounds from the right side
    }
    if (y() + boundingRect.height() > 390) {
        setY(390 - boundingRect.height());  // Ensure it doesn’t go below the grid
    }
}

// Call this after rotation:
void brick::keyPressEvent(QKeyEvent* k) {
    switch (k->key()) {
        case Qt::Key_Left:
            setX(x() - tile_size);
            break;
        case Qt::Key_Right:
            setX(x() + tile_size);
            break;
        case Qt::Key_Up:
            this->setRotation(rotation() + 90);
            adjustAfterRotation();  // Adjust brick if it rotates out of bounds
            break;
    }
}
