#include "board.h"


board::board(QGraphicsScene* scene, QGraphicsView* view) {
	int boardX = 28;
	int boardY = 30;
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 10; j++) {
			tile* activeTile = new tile(boardX, boardY);
			boardX += tileSize;

			gameArea[i][j] = activeTile;
			scene->addItem(activeTile->getPtr());
		}
		boardX = 28;
		boardY += tileSize;
	}
}


void board::changeTile(int i, int j, int colour) {
	QPixmap tileset(":/resources/textures/tiles.png");
	QRect rectangle(colour * tileSize, 0, tileSize, tileSize);
	QPixmap cutTexture = tileset.copy(rectangle);
	
	gameArea[i][j]->getPtr()->setPixmap(cutTexture);
}