#include "NetworkMessageManager.h"
#include <QHostAddress>
#include <QDebug>
#include "board.h"

NetworkMessageManager::NetworkMessageManager(board* boardGame, QObject* parent)
    : QObject(parent), inputServer(nullptr), inputSocket(nullptr), outputSocket(nullptr),
    nextNeighborPort(0), previousNeighborPort(0) {
    game_board = boardGame;
}

NetworkMessageManager::~NetworkMessageManager() {
    stopWorking();
}

void NetworkMessageManager::setInputParameters(quint16 port) {
    previousNeighborPort = port;
}

void NetworkMessageManager::setOutputParameters(const QString& ip, quint16 port) {
    nextNeighborIP = ip;
    nextNeighborPort = port;
}

void NetworkMessageManager::startListeningToPrevious() {
    if (!inputServer) {
        inputServer = new QTcpServer();
        connect(inputServer, &QTcpServer::newConnection, this, [this]() {
            inputSocket = inputServer->nextPendingConnection();
            connect(inputSocket, &QTcpSocket::readyRead, this, &NetworkMessageManager::onReadyRead);
            qDebug() << "Connected to previous neighbor on port" << previousNeighborPort;
            });

        if (inputServer->listen(QHostAddress::Any, previousNeighborPort)) {
            qDebug() << "Listening for previous neighbor on port" << previousNeighborPort;
        }
        else {
            qDebug() << "Failed to listen on port" << previousNeighborPort << ":" << inputServer->errorString();
        }
    }
}

void NetworkMessageManager::startConnectingToNext() {
    if (!outputSocket) {
        outputSocket = new QTcpSocket();
        outputSocket->connectToHost(nextNeighborIP, nextNeighborPort);
        if (outputSocket->waitForConnected(5000)) {
            qDebug() << "Connected to next neighbor at" << nextNeighborIP << ":" << nextNeighborPort;
        }
        else {
            qDebug() << "Failed to connect to next neighbor at" << nextNeighborIP << ":" << nextNeighborPort;
        }
    }
}

void NetworkMessageManager::stopWorking() {
    if (inputSocket) {
        inputSocket->close();
        inputSocket->deleteLater();
        inputSocket = nullptr;
    }
    if (inputServer) {
        inputServer->close();
        inputServer->deleteLater();
        inputServer = nullptr;
    }
    if (outputSocket) {
        outputSocket->close();
        outputSocket->deleteLater();
        outputSocket = nullptr;
    }
    qDebug() << "NetworkMessageManager stopped.";
}

void NetworkMessageManager::proceedMessageFromPrevious(const NetworkMessage& message) {
    qDebug() << QString("Player id:%1 is processing message. Message: sender:%2, message content:%3, sentTime:%4, typeOfMessage:%5")
        .arg(localPlayer.getId())
        .arg(message.getSender().getId())
        .arg(message.getText())
        .arg(message.getSendTime().toString())
        .arg(message.getType());
    if (message.getType() == NetworkMessage::deleteRow) {
        serveDeleteRowMessage(message);
    }
    else if (message.getType() == NetworkMessage::canRowBeDeleted) {
        serveCanRowBeDeletedMessage(message);
    }
    else {
        qDebug() << "Wrong type of message";
    }
}

void NetworkMessageManager::sendMessageToNext(const NetworkMessage& message) {
    if (outputSocket && outputSocket->isWritable()) {
        QByteArray data = message.toByteArray();
        outputSocket->write(data);
        outputSocket->flush();
        qDebug() << QString("Player id:%1 is sending message. Message: sender:%2, message content:%3, sentTime:%4, typeOfMessage:%5")
            .arg(localPlayer.getId())
            .arg(message.getSender().getId())
            .arg(message.getText())
            .arg(message.getSendTime().toString())
            .arg(message.getType());
    }
    else {
        qDebug() << "Output socket is not writable.";
    }
}

void NetworkMessageManager::setLocalPlayer(const Player& localPlayerVal) {
    localPlayer.setId(localPlayerVal.getId());
    localPlayer.setIpAddress(localPlayerVal.getIpAddress());
}

Player NetworkMessageManager::getLocalPlayer() {
    return localPlayer;
}

void NetworkMessageManager::onReadyRead() {
    if (inputSocket && inputSocket->bytesAvailable() > 0) {
        QByteArray data = inputSocket->readAll();
        NetworkMessage message = NetworkMessage::fromByteArray(data);
        proceedMessageFromPrevious(message);
    }
}

void NetworkMessageManager::serveDeleteRowMessage(const NetworkMessage& message) {
    if (message.getSender().getId() != localPlayer.getId()) {

        int lineId = message.getText().toInt();
        game_board->delete_line(lineId);
        game_board->move_all_down(lineId);
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
    }
}

void NetworkMessageManager::serveCanRowBeDeletedMessage(const NetworkMessage& message) {
    // Implementacja logiki sprawdzania usuniecia wiersza
}
