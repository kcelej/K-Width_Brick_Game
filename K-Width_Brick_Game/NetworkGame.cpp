#include "NetworkGame.h"

NetworkGame::NetworkGame() {
	localPlayer.setIpAddress(NetworkUtils::getLocalIP());
	networkMessageManager.setLocalPlayer(localPlayer);
}

void NetworkGame::configNetworkNeighbors(QString nextNeighborIP, quint16 nextNeighborPort, quint16 previousNeighborPort) {
	networkMessageManager.setInputParameters(previousNeighborPort);
	networkMessageManager.setOutputParameters(nextNeighborIP, nextNeighborPort);
	qDebug() << "network configured";
}

void NetworkGame::startListening() {
	networkMessageManager.startListeningToPrevious();
}

void NetworkGame::connectToNext() {
	networkMessageManager.startConnectingToNext();
}

void  NetworkGame::setUserId(int id) {
	qDebug() << "id set to " << id;
	localPlayer.setId(id);
	networkMessageManager.setLocalPlayer(localPlayer);
}

NetworkGame::~NetworkGame() {

}
void NetworkGame::startGame() {
	QString message = QString("Player nr %1 started game").arg(localPlayer.getId());
	if (localPlayer.getId() == 1) {
		networkMessageManager.sendMessageToNext(NetworkMessage(localPlayer, message, QTime::currentTime(), NetworkMessage::canRowBeDeleted));
	}
}