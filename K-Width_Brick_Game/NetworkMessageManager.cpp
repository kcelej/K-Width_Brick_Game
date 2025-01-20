#include "NetworkMessageManager.h"
#include <QHostAddress>
#include <QDebug>
#include "board.h"
#include "NetworkMessageFactory.h"
#include "Tetris_Game.h"

NetworkMessageManager::NetworkMessageManager(Tetris_Game* aGame, QObject* parent)
    : QObject(parent), inputServer(nullptr), inputSocket(nullptr), outputSocket(nullptr),
    nextNeighborPort(0), previousNeighborPort(0), priorityPlayer(nullptr),isListenOnlyOnePlayer(false),
    priorityMessageSendTime(nullptr), everyOneListenMe(false), localPlayerSentListenOnlyMeMessage(false){
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
    qDebug() << QString("Player id:%1 finish processing message. Message: sender:%2, message content:%3, sentTime:%4, typeOfMessage:%5")
        .arg(localPlayer.getId())
        .arg(message.getSender().getId())
        .arg(message.getText())
        .arg(message.getSendTime().toString())
        .arg(message.getType());
}

void NetworkMessageManager::sendMessageToNext(const NetworkMessage message) {
    std::mutex m;
    m.lock();
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
    m.unlock();
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
            qDebug("All players ready to remove line");
            game_board->delete_line(messsageInfo.numberOfRow);
            qDebug("delete line");
            NetworkMessage deleteMessage = NetworkMessageFactory::createDeleteRowMessage(localPlayer, messsageInfo.numberOfRow);
            sendMessageToNext(deleteMessage);
            qDebug("message sent");
            waitForMessageFinish(deleteMessage);
            qDebug("Wait for message delete");
            game_board->move_all_down(messsageInfo.numberOfRow);
            qDebug("move all down");
            NetworkMessage moveAllDownMessage = NetworkMessageFactory::createMoveAllDownMessage(localPlayer, messsageInfo.numberOfRow);
            sendMessageToNext(moveAllDownMessage);
            qDebug("message sent");
            waitForMessageFinish(moveAllDownMessage);
            qDebug("Wait for messafe move all down");
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
    qDebug("serve listen only me");

    // SprawdŸ, czy wiadomoœæ pochodzi od innego gracza
    if (message.getSender().getId() != localPlayer.getId()) {
        qDebug("Serving 'listen only me' message from different player");

        if (isListenOnlyOnePlayer|| localPlayerSentListenOnlyMeMessage) {
            qDebug("Already listening to one player");

            // Porównaj czas wiadomoœci
            if (priorityMessageSendTime) {
                if (message.getSendTime() < *priorityMessageSendTime) {
                    qDebug("New message is older. Updating priority.");
                    *priorityPlayer = message.getSender();
                    *priorityMessageSendTime = message.getSendTime();
                    sendMessageToNext(message);
                    game->freeze();
                    isListenOnlyOnePlayer = true;
                }
                else if (message.getSendTime() == *priorityMessageSendTime) {
                    qDebug("Messages have the same time. Resolving by sender ID.");
                    // Rozstrzyganie przypadku równego czasu na podstawie ID gracza
                    if (message.getSender().getId() < priorityPlayer->getId()) {
                        qDebug("New sender has lower ID. Updating priority.");
                        *priorityPlayer = message.getSender();
                        sendMessageToNext(message);
                        game->freeze();
                        isListenOnlyOnePlayer = true;
                        localPlayerSentListenOnlyMeMessage = false;
                    }
                    else {
                        qDebug("Current priority remains. Ignoring new message.");
                    }
                }
                else {
                    qDebug("New message is newer. Ignoring.");
                }
            }
        }
        else {
            qDebug("Not currently listening to any player. Setting new priority.");
            // Ustawienie nowego gracza i czasu priorytetowego
            if (!priorityPlayer) {
                priorityPlayer = new Player();
            }
            if (!priorityMessageSendTime) {
                priorityMessageSendTime = new QTime();
            }
            *priorityPlayer = message.getSender();
            *priorityMessageSendTime = message.getSendTime();

            isListenOnlyOnePlayer = true;
            sendMessageToNext(message);
            game->freeze();
            localPlayerSentListenOnlyMeMessage = false;
        }
    }
    else {
        qDebug("Loop detected. Stop propagation.");
        everyOneListenMe = true;
    }
}

void NetworkMessageManager::setPriorityMessageBeforeSendingListenOlnyMeMessage(const NetworkMessage& message) {
    if (!priorityPlayer) {
        priorityPlayer = new Player();
    }
    if (!priorityMessageSendTime) {
        priorityMessageSendTime = new QTime();
    }
    *priorityPlayer = message.getSender();
    *priorityMessageSendTime = message.getSendTime();
    localPlayerSentListenOnlyMeMessage = true;
}

void NetworkMessageManager::serveStopListenOnlyMeMessage(const NetworkMessage& message) {
    qDebug("stop listen me message");
    if (message.getSender().getId() != localPlayer.getId()) {
        qDebug("\n\n\nserving stop listen only me message");
        qDebug("diff id");
        isListenOnlyOnePlayer = false;
        localPlayerSentListenOnlyMeMessage = false;
        if (priorityMessageSendTime != nullptr && priorityPlayer != nullptr) {
            delete priorityMessageSendTime;
            delete priorityPlayer;
        }
        priorityMessageSendTime = nullptr;
        priorityPlayer = nullptr;
        game->unFreeze();
        sendMessageToNext(message);
    }
    else {
        qDebug() << "Loop detected, stop propagation";
        everyOneListenMe = false;
        localPlayerSentListenOnlyMeMessage = false;
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
    while (!(lastMessage == message)) {
        QApplication::processEvents();
    }
    qDebug("wait for message end");
}

void NetworkMessageManager::waitForMessageFinish(const NetworkMessage& message) {
    qDebug("wait for message finish beg");
    while (!(lastFinishMessage == message)) {
        QThread::msleep(100);
        QApplication::processEvents();
        onReadyRead();
    }
    qDebug("wait for message finish end");
}

