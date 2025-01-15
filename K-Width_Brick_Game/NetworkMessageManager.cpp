#include "NetworkMessageManager.h"
#include <QHostAddress>
#include <QDebug>
#include "board.h"
#include "NetworkMessageFactory.h"
#include "Tetris_Game.h"

NetworkMessageManager::NetworkMessageManager(Tetris_Game* aGame, QObject* parent)
    : QObject(parent), inputServer(nullptr), inputSocket(nullptr), outputSocket(nullptr),
    nextNeighborPort(0), previousNeighborPort(0), priorityPlayer(nullptr),isListenOnlyOnePlayer(false),
    priorityMessageSendTime(nullptr), everyOneListenMe(false){
    game = aGame;
    game_board = game->getBoard();
}

NetworkMessageManager::~NetworkMessageManager() {
    stopWorking();
    delete priorityMessageSendTime;
    delete priorityPlayer;
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
    lastMessage = message;
    emit messageReceived(lastMessage);

    if (message.getType() == NetworkMessage::deleteRow) {
        serveDeleteRowMessage(message);
    }
    else if (message.getType() == NetworkMessage::canRowBeDeleted) {
        serveCanRowBeDeletedMessage(message);
    }
    else if (message.getType() == NetworkMessage::moveAllDown) {
        serveMoveAllDownMessage(message);
    }
    else if (message.getType() == NetworkMessage::listenOnlyMe) {
        serveListenOnlyMeMessage(message);
    }
    else if (message.getType() == NetworkMessage::stopListenOnlyMe) {
        serveStopListenOnlyMeMessage(message);
    }
    else {
        qDebug() << "Wrong type of message";
    }
    lastFinishMessage = message;
    emit messageFinished(lastFinishMessage);
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
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
    }
}

void NetworkMessageManager::serveCanRowBeDeletedMessage(const NetworkMessage& message) {
    NetworkMessage::CanRowBeDeletedMessageInfo messsageInfo = NetworkMessage::CanRowBeDeletedMessageInfo::fromString(message.getText());

    if (message.getSender().getId() != localPlayer.getId()) {

        int lineId = message.getText().toInt();
        bool isFullLine = game_board->check_line_condition(messsageInfo.numberOfRow, true);
        messsageInfo.playersRowState[localPlayer.getId() - 1] = isFullLine;
        NetworkMessage newMessage(message.getSender(), messsageInfo.toString(), message.getSendTime(), message.getType());
        sendMessageToNext(newMessage);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
        bool areAllPlayersReadyToRemoveRow = true;
        for (bool rowValue : messsageInfo.playersRowState) {
            if (rowValue == false) {
                areAllPlayersReadyToRemoveRow = false;
            }
        }
        if (areAllPlayersReadyToRemoveRow) {
            game_board->delete_line(messsageInfo.numberOfRow);
            NetworkMessage deleteMessage = NetworkMessageFactory::createDeleteRowMessage(localPlayer, messsageInfo.numberOfRow);
            sendMessageToNext(deleteMessage);
            //waitForMessage(deleteMessage);
            game_board->move_all_down(messsageInfo.numberOfRow);
            NetworkMessage moveAllDownMessage = NetworkMessageFactory::createMoveAllDownMessage(localPlayer, messsageInfo.numberOfRow);
            sendMessageToNext(moveAllDownMessage);
           // waitForMessage(moveAllDownMessage);
        }
    }
}

void NetworkMessageManager::serveMoveAllDownMessage(const NetworkMessage& message) {
    if (message.getSender().getId() != localPlayer.getId()) {
        int lineId = message.getText().toInt();
        game_board->move_all_down(lineId);
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
    }

}

void NetworkMessageManager::serveListenOnlyMeMessage(const NetworkMessage& message) {
    if (message.getSender().getId() != localPlayer.getId()) {
        if (isListenOnlyOnePlayer) {
            if (!priorityMessageSendTime && (message.getSendTime() < *priorityMessageSendTime)) {
                *priorityPlayer = message.getSender();
                *priorityMessageSendTime = message.getSendTime();
                sendMessageToNext(message);
                game->freeze();
            }
        }
        else {
            priorityPlayer = new Player();
            priorityMessageSendTime = new QTime();
            *priorityPlayer = message.getSender();
            *priorityMessageSendTime = message.getSendTime();
            sendMessageToNext(message);
            game->freeze();
            isListenOnlyOnePlayer = true;
        }
    }
    else {
        qDebug() << "Loop detected, stop propagation";
        everyOneListenMe = true;
    }

}
void NetworkMessageManager::serveStopListenOnlyMeMessage(const NetworkMessage& message) {
    if (message.getSender().getId() != localPlayer.getId()) {
        isListenOnlyOnePlayer = false;
        delete priorityMessageSendTime;
        delete priorityPlayer;
        priorityMessageSendTime = nullptr;
        priorityPlayer = nullptr;
        game->unFreeze();
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
        everyOneListenMe = false;
    }

}

bool  NetworkMessageManager::isEveryOneListenMe() {
    return everyOneListenMe;
}
bool  NetworkMessageManager::isListenOlnyOtherPlayer() {
    return isListenOnlyOnePlayer;
}

void NetworkMessageManager::waitForMessage(const NetworkMessage& message) {

    qDebug("wait for message beg");

    QEventLoop loop;
    bool messageMatched = false;

 
    connect(this, &NetworkMessageManager::messageReceived, this, [&](const NetworkMessage& receivedMessage) {
        if (receivedMessage == message) {
            messageMatched = true;
            loop.quit(); 
        }
        });

    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, &loop, [&]() {
        qDebug("Timeout reached while waiting for message");
        loop.quit();
        });
    timer.start(5000); 

    loop.exec(); 

    if (messageMatched) {
        qDebug("Expected message received");
    }
    else {
        qDebug("Wait for message ended without receiving the expected message");
    }

    qDebug("wait for message end");
}

void NetworkMessageManager::waitForMessageFinish(const NetworkMessage& message) {

    qDebug("wait for message finish beg");

    QEventLoop loop;
    bool messageMatched = false;

    auto connection = connect(this, &NetworkMessageManager::messageFinished, this, [&](const NetworkMessage& finishedMessage) {
        if (finishedMessage == message) {
            messageMatched = true;
            loop.quit();
        }
        }, Qt::QueuedConnection);

    QTimer timer;
    timer.setSingleShot(true);
    connect(&timer, &QTimer::timeout, [&]() {
        qDebug("Timeout reached while waiting for message");
        loop.quit();
        });

    timer.start(5000); 

    loop.exec();

    disconnect(connection);

    if (messageMatched) {
        qDebug("Expected message finished");
    }
    else {
        qDebug("Wait for message ended without receiving the expected message");
    }

    qDebug("wait for message finish end");
}

