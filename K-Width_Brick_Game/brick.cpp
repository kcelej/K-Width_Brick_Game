#include "brick.h"
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QRect>

brick::brick(int t_s, int c) : tile_size(t_s), color(c) {
    draw();

    // Set the starting position of the brick within the game field (centered at the top)
    this->setPos(4 * tile_size + 28, 30-tile_size);  // 28 is the left boundary, 30 is the top boundary

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

    // Ensure the brick stays within bounds after rotation
    if (x() < 28) {
        setX(28);  // Left boundary
    }
    if (x() + boundingRect.width() > 208) {
        setX(208 - boundingRect.width());  // Right boundary adjustment for brick's width
    }
    if (y() + boundingRect.height() > 390) {
        setY(390 - boundingRect.height());  // Bottom boundary
    }

    // Snap to grid by adjusting position to the nearest multiple of tile_size
    int newX = static_cast<int>((x() - 28) / tile_size) * tile_size + 28;
    int newY = static_cast<int>((y() - 30) / tile_size) * tile_size + 30;
    setPos(newX, newY);
}



void brick::keyPressEvent(QKeyEvent* k) {
    switch (k->key()) {
    case Qt::Key_Left:
        if (x() - tile_size >= 28)  // Left boundary (28px)
            setX(x() - tile_size);
        break;
    case Qt::Key_Right:
        if (x() + tile_size < 180)  // Right boundary (208px - tile size)
            setX(x() + tile_size);
        break;
    case Qt::Key_Up:
        this->setRotation(rotation() + 90);
        adjustAfterRotation();  // Adjust brick if it rotates out of bounds
        break;
    }
}

