//#include "NetworkMessageManager.h"
//#include <QHostAddress>
//#include <QDebug>
//
//NetworkMessageManager::NetworkMessageManager(QObject* parent)
//    : QObject(parent), inputSocket(nullptr), outputSocket(nullptr) {
//    nextNeighborPort = 0;
//    previousNeighborPort = 0;
//    nextNeighborIP = "";
//}
//
//void NetworkMessageManager::setInputParameters(quint16 port) {
//    previousNeighborPort = port;
//}
//
//void NetworkMessageManager::setOutputParameters(const QString& ip, quint16 port) {
//    nextNeighborIP = ip;
//    nextNeighborPort = port;
//}
//
//void NetworkMessageManager::startWorking() {
//    inputSocket = new QTcpSocket(this);
//    inputSocket->bind(QHostAddress::Any, previousNeighborPort);
//    connect(inputSocket, &QTcpSocket::readyRead, this, &NetworkMessageManager::onReadyRead);
//    outputSocket = new QTcpSocket(this);
//    outputSocket->connectToHost(nextNeighborIP, nextNeighborPort);
//    if (!outputSocket->waitForConnected(5000)) { // czekaj maksymalnie 5 sekund
//        qDebug() << "Failed to connect to host:" << outputSocket->errorString();
//        return;
//    }
//    if (!inputSocket->waitForConnected(5000)) { // czekaj maksymalnie 5 sekund
//        qDebug() << "Failed to connect to host:" << outputSocket->errorString();
//        return;
//    }
//    qDebug() << "networking started";
//}
//
//void NetworkMessageManager::stopWorking() {
//    if (inputSocket) {
//        inputSocket->close();
//        inputSocket->deleteLater();
//    }
//    if (outputSocket) {
//        outputSocket->close();
//        outputSocket->deleteLater();
//    }
//    qDebug() << "NetworkMessageManager has stopped.";
//}
//
//void NetworkMessageManager::proceedMessageFromPrevious(const NetworkMessage& message) {
//
//    qDebug() << "The player of id = " << localPlayer.getId() << " and ip = " << localPlayer.getIpAddress()
//        << " is processing incoming message: " << message.toByteArray();
//
//    //switch (message.getType())
//    //{
//    //case NetworkMessage::deleteRow:
//    //    serveDeleteRowMessage(message);
//    //    break;
//    //case NetworkMessage::canRowBeDeleted:
//    //    serveCanRowBeDeletedMessage(message);
//    //    break;
//    //default:
//    //    break;
//    //    qDebug() << "Wrong kind of message";
//    //}
//
//    //test
//    if (message.getSender().getId() != localPlayer.getId()) {
//        sendMessageToNext(message);
//    }
//}
//
//void NetworkMessageManager::serveDeleteRowMessage(const NetworkMessage& message) {
//    if (message.getSender().getId() != localPlayer.getId()) {
//        //usuwanie wiersza
//        //wyslanie komunukatu o usunieciu dalej
//    }
//}
//void NetworkMessageManager::serveCanRowBeDeletedMessage(const NetworkMessage& message) {
//    if (message.getSender().getId() == localPlayer.getId()) {
//        //sprawdzenie stanu odpowiedzi
//        //ewentualne usuniecie wiersza
//        //jesli usuniecie to wyslaniue komunikatu o usunieciu
//    }
//    else {
//        //sprawdzenie stanu wiersza
//        //zaktualizowanie wiadomosci
//        //wyslanie dalej
//    }
//}
//
//void NetworkMessageManager::setLocalPlayer(const Player& localPlayerVal) {
//    localPlayer.setId(localPlayerVal.getId());
//    localPlayer.setIpAddress(localPlayerVal.getIpAddress());
//}
//
//void NetworkMessageManager::sendMessageToNext(const NetworkMessage& message) {
//    qDebug() << "send to next begin";
//    if (outputSocket && outputSocket->isWritable()) {
//        QByteArray data = message.toByteArray();
//        outputSocket->write(data);
//        outputSocket->flush();
//    }
//    qDebug() << "send to next end";
//}
//
//void NetworkMessageManager::onReadyRead() {
//    if (inputSocket && inputSocket->bytesAvailable() > 0) {
//        QByteArray data = inputSocket->readAll();
//        proceedMessageFromPrevious(NetworkMessage::fromByteArray(data));
//    }
//}
#include "NetworkMessageManager.h"
#include <QHostAddress>
#include <QDebug>

NetworkMessageManager::NetworkMessageManager(QObject* parent)
    : QObject(parent), inputServer(nullptr), inputSocket(nullptr), outputSocket(nullptr),
    nextNeighborPort(0), previousNeighborPort(0) {
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
        inputServer = new QTcpServer(this);
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
        outputSocket = new QTcpSocket(this);
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
    //qDebug() << QString("Processing message from player %1 to %2. Text: %3, Type: %4, SentTime: %5").arg(message.getSender().getId())
    //    .arg(localPlayer.getId()).arg(message.getText()).arg(message.getType()).arg(message.getSendTime().toString());

    qDebug() << QString("Player id:%1 is processing message. Message: sender:%2, message content:%3, sentTime:%4, typeOfMessage:%5")
        .arg(localPlayer.getId())
        .arg(message.getSender().getId())
        .arg(message.getText())
        .arg(message.getSendTime().toString())
        .arg(message.getType());

    if (message.getSender().getId() != localPlayer.getId()) {
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Message loop detected. Stopping propagation.";
    }
}

void NetworkMessageManager::sendMessageToNext(const NetworkMessage& message) {
    if (outputSocket && outputSocket->isWritable()) {
        QByteArray data = message.toByteArray();
        outputSocket->write(data);
        outputSocket->flush();
        qDebug() << QString("Player id:%1 sent message to next").arg(localPlayer.getId());
    }
    else {
        qDebug() << "Output socket is not writable.";
    }
}

void NetworkMessageManager::setLocalPlayer(const Player& localPlayerVal) {
    localPlayer.setId(localPlayerVal.getId());
    localPlayer.setIpAddress(localPlayerVal.getIpAddress());
}

void NetworkMessageManager::onReadyRead() {
    if (inputSocket && inputSocket->bytesAvailable() > 0) {
        QByteArray data = inputSocket->readAll();
        NetworkMessage message = NetworkMessage::fromByteArray(data);
        proceedMessageFromPrevious(message);
    }
}

void NetworkMessageManager::serveDeleteRowMessage(const NetworkMessage& message) {
    // Implementacja logiki usuwania wiersza

}

void NetworkMessageManager::serveCanRowBeDeletedMessage(const NetworkMessage& message) {
    // Implementacja logiki sprawdzania usuniêcia wiersza
}
