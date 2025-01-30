#pragma once
#include "board.h"
#include "NetworkMessageFactory.h"

board::board(QGraphicsScene* scene) {
	int boardX = 28;
	int boardY = 30;

	// Create the game grid with tiles.
	for (int i = 0; i < GAME_AREA_HEIGHT; i++) {
		for (int j = 0; j < GAME_AREA_WIDTH; j++) {
			tile* activeTile = new tile(boardX, boardY);
			boardX += TILE_SIZE;

			// Store the tile in the game board array.
			gameArea[i][j] = activeTile;
			// Add tile to the scene.
			scene->addItem(activeTile->getPtr());
		}
		boardX = 28;
		boardY += TILE_SIZE;
	}

}

void board::changeTile(int i, int j, int color) {
	if (i >= 0 && i < GAME_AREA_HEIGHT && j >= 0 && j < GAME_AREA_WIDTH && gameArea[i][j] != nullptr) {
		QPixmap tileset(":/resources/textures/tiles.png");
		if (!tileset.isNull()) {
			QRect rectangle(color * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
			QPixmap cutTexture = tileset.copy(rectangle);
			gameArea[i][j]->getPtr()->setPixmap(cutTexture);
			gameArea[i][j]->set_color(color);
		}
		else {
			qWarning("Failed to load the tileset in changeTile.");
		}
	}
	else {
		qWarning("Invalid access to gameArea[%d][%d] in changeTile", i, j);
	}
}


bool board::isTileTaken(int i, int j) {
	return gameArea[i][j]->getIsOccupied();
}

void board::changeTileStatus(int i, int j, bool b) {
	gameArea[i][j]->changeTileStatus(b);
}

void board::resetTile(int i, int j) {
	if (i >= 0 && i < GAME_AREA_HEIGHT && j >= 0 && j < GAME_AREA_WIDTH && gameArea[i][j] != nullptr) {
		QPixmap tileset(":/resources/textures/empty_tile.png");
		if (!tileset.isNull()) {
			QRect rectangle(TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
			QPixmap cutTexture = tileset.copy(rectangle);
			gameArea[i][j]->getPtr()->setPixmap(cutTexture);
			changeTileStatus(i, j, false);
		}
		else {
			qWarning("Failed to load the empty tile texture in resetTile.");
		}
	}
	else {
		qWarning("Invalid access to gameArea[%d][%d] in resetTile", i, j);
	}
}

bool board::check_line_condition(int i, bool check_for_occupied) {
	for (int j = 0; j < GAME_AREA_WIDTH; j++) {
		bool is_occupied = gameArea[i][j]->getIsOccupied();
		if (check_for_occupied && !is_occupied) {
			return false; // The line contains empty spaces.
		}
		if (!check_for_occupied && is_occupied) {
			return false; // The line contains bricks.
		}
	}
	qDebug() << (check_for_occupied ? "Full line occupied." : "Full line empty.");
	return true;
}

void board::delete_line(int i) {
	qDebug() << "Line " << i << " removed.";
	for (int j = 0; j < GAME_AREA_WIDTH; j++) {
		resetTile(i, j);
	}
	//setLineStatus(i, false);
}

void board::move_all_down(int i) {
	//std::lock_guard<std::mutex> lock(boardMutex);
	qDebug() << "Moving the lines. i:"<<i;
	try
	{
		for (int row = i; row >= 0; row--) {
			qDebug() << "row nr " << row;
			if (row - 1 < 0 || check_line_condition(row - 1, false)) {
				delete_line(row);
				break;
			}
			else {
				for (int j = 0; j < GAME_AREA_WIDTH; j++) {
					if (gameArea[row - 1][j]->getIsOccupied()) {
						changeTileStatus(row, j, true);
						changeTile(row, j, gameArea[row - 1][j]->get_color());
					}
					else resetTile(row, j);
				}
			}
		}
	}
	catch (const std::exception& e)
	{
		qDebug() << e.what();
	}
}

void board::check_board() {
	qDebug() << "\n\n\nChecking the board.";
	gameView->setDisabled(true);
	// Start checking from the bottom-most row and move upward.
	NetworkMessage listenOnlyMeMessage = NetworkMessageFactory::createListenOnlyMeMessage(networkMessageManager->getLocalPlayer());
	networkMessageManager->setPriorityMessageBeforeSendingListenOlnyMeMessage(listenOnlyMeMessage);
	notifyOtherNetworkPlayers(listenOnlyMeMessage);
	while (true) {
		if (networkMessageManager->isEveryOneListenMe()) {
			qDebug("every one listen me, break from check board");
			break;
		}
		if (networkMessageManager->isListenOlnyOtherPlayer()) {
			qDebug("listen other player, return from check board");
			return;
		}
		QCoreApplication::processEvents();
	}
	for (int i = GAME_AREA_HEIGHT - 1; i >= 0; i--) {
	
		if (check_line_condition(i, true)) {
			qDebug() << "Condition met for row:" << i;

			try {
				NetworkMessage message = NetworkMessageFactory::createCanRowBeDeletedMessage(
					networkMessageManager->getLocalPlayer(), i, true
				);

				qDebug() << "Message created for row:" << i;

				notifyOtherNetworkPlayers(message);

				qDebug() << "Waiting for message finish for row:" << i;

				networkMessageManager->waitForMessageFinish(message);

				qDebug() << "Finished processing row:" << i;
			}
			catch (const std::exception& e) {
				qDebug() << "Exception for row:" << i << "Exception:" << e.what();
			}
		}
		else {
			qDebug() << "Condition not met for row:" << i;
		}
	}
	NetworkMessage message = NetworkMessageFactory::createStopListenOnlyMeMessage(networkMessageManager->getLocalPlayer());
	notifyOtherNetworkPlayers(message);
	networkMessageManager->waitForMessage(message);
	gameView->setDisabled(false);
	qDebug("checking board finished\n\n\n");
}

void board::setNetworkMessageManager(NetworkMessageManager* networkMessageManagerVal) {
	networkMessageManager = networkMessageManagerVal;
}

void board::notifyOtherNetworkPlayers(NetworkMessage message) {
	qDebug() << "board::notifyOtherNetworkPlayers";
	if (networkMessageManager) {
		networkMessageManager->sendMessageToNext(message);
	}
}

void board::setLineStatus(int lineNumber, bool checked) {
	linesStatus[lineNumber] = checked;
}

void board::setGameView(QGraphicsView* view) {
	this->gameView = view;
}