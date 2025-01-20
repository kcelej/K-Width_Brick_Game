#pragma once
#include "board.h"
#include "NetworkMessageFactory.h"

board::board(QGraphicsScene* scene) {
	int boardX = 28;
	int boardY = 30;
	for (int i = 0; i < GAME_AREA_HEIGHT; i++) {
		for (int j = 0; j < GAME_AREA_WIDTH; j++) {
			tile* activeTile = new tile(boardX, boardY);
			boardX += TILE_SIZE;

			gameArea[i][j] = activeTile;
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

//fix needed?
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
	//qDebug() << "Checking the line with condition:" << (check_for_occupied ? "Occupied" : "Empty");
	for (int j = 0; j < GAME_AREA_WIDTH; j++) {
		bool is_occupied = gameArea[i][j]->getIsOccupied();
		if (check_for_occupied && !is_occupied) {
			//qDebug() << "The line contains blanks.";
			return false; // The line is not fully occupied.
		}
		if (!check_for_occupied && is_occupied) {
			//qDebug() << "The line contains bricks.";
			return false; // The line is not fully empty.
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

	//qDebug() << "Moving all lines down from row:" << i;

	//try {
	//	std::lock_guard<std::mutex> lock(boardMutex);

	//	for (int row = i; row > 0; row--) {
	//		for (int j = 0; j < GAME_AREA_WIDTH; j++) {
	//			if (gameArea[row - 1][j]->getIsOccupied()) {
	//		
	//				changeTileStatus(row, j, true);  
	//				changeTile(row, j, gameArea[row - 1][j]->get_color());  
	//			}
	//			else {
	//				resetTile(row, j); 
	//			}
	//		}
	//	}
	//}
	//catch (const std::exception& e) {
	//	qDebug() << "Exception occurred while moving lines down: " << e.what();
	//}

}

void board::check_board() {
	
	//qDebug() << "Checking the board.";

	//// Wy�lij wiadomo��, �e lokalny gracz chce przej�� kontrol�
	//notifyOtherNetworkPlayers(NetworkMessageFactory::createListenOnlyMeMessage(networkMessageManager->getLocalPlayer()));

	//// Oczekiwanie, a� wszyscy gracze b�d� s�ucha� lokalnego gracza
	//while (true) {
	//	{
	//		std::lock_guard<std::mutex> lock(boardMutex);

	//		if (networkMessageManager->isEveryOneListenMe()) {
	//			qDebug() << "Everyone is listening to me.";
	//			break;
	//		}
	//		if (networkMessageManager->isListenOlnyOtherPlayer()) {
	//			qDebug() << "Another player is listening. Exiting check_board.";
	//			return;
	//		}
	//	}
	//	QCoreApplication::processEvents(); 
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
	//}

	//for (int i = GAME_AREA_HEIGHT - 1; i >= 0; i--) {
	//	qDebug() << "Checking row:" << i;

	//	if (check_line_condition(i, true)) {
	//		qDebug() << "Row " << i << " is fully occupied.";

	//		try {
	//
	//			NetworkMessage message = NetworkMessageFactory::createCanRowBeDeletedMessage(
	//				networkMessageManager->getLocalPlayer(), i, true
	//			);

	//
	//			notifyOtherNetworkPlayers(message);

	//	
	//			networkMessageManager->waitForMessageFinish(message);

	//			move_all_down(i);
	//		}
	//		catch (const std::exception& e) {
	//			qDebug() << "Exception while processing row " << i << ": " << e.what();
	//		}
	//	}
	//	else {
	//		qDebug() << "Row " << i << " is not fully occupied.";
	//	}
	//}

	//// Wy�lij wiadomo��, �e lokalny gracz ko�czy kontrol�
	//NetworkMessage message = NetworkMessageFactory::createStopListenOnlyMeMessage(networkMessageManager->getLocalPlayer());
	//notifyOtherNetworkPlayers(message);

	//qDebug() << "Finished checking the board.";

	qDebug() << "\n\n\nChecking the board.";
	gameView->setDisabled(true);
	// Start checking from the bottom-most row and move upward.
	notifyOtherNetworkPlayers(NetworkMessageFactory::createListenOnlyMeMessage(networkMessageManager->getLocalPlayer()));
	while (true) {
		if (networkMessageManager->isEveryOneListenMe()) {
			break;
		}
		if (networkMessageManager->isListenOlnyOtherPlayer()) {
			return;
		}
		QCoreApplication::processEvents();
	}
	//if(potwierdzone){}
	//else if slcuha kogos innego return
	for (int i = GAME_AREA_HEIGHT - 1; i >= 0; i--) {
		// Continue checking the same row while it is completely filled.
		//while (check_line_condition(i, true)) {
		//	// The line is full and can be removed.
		//	delete_line(i);
		//	notifyOtherNetworkPlayers(NetworkMessageFactory::createDeleteRowMessage(networkMessageManager->getLocalPlayer(), i));
		//	// Shift all lines above the deleted line one row down.
		//	move_all_down(i);
		//}
		//jesli gracz lokalnie stwierdzi, ze moze usunac wiersz, to wysyla wiadomosc do inncyh z zapytaniem, czy inni tez moga
	/*	if (check_line_condition(i, true)) {
			if (linesStatus[i] == false) {
			notifyOtherNetworkPlayers(NetworkMessageFactory::createCanRowBeDeletedMessage(networkMessageManager->getLocalPlayer(), i, true));
				setLineStatus(i, true);
			}
		}*/
		/*if (check_line_condition(i, true)) {

			NetworkMessage message = NetworkMessageFactory::createCanRowBeDeletedMessage(networkMessageManager->getLocalPlayer(), i, true);
			notifyOtherNetworkPlayers(message);
			networkMessageManager->waitForMessageFinish(message);
		}*/

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