#ifndef NETWORKMESSAGEMANAGER_H
#define NETWORKMESSAGEMANAGER_H

#include "NetworkMessage.h"
#include "Player.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class Tetris_Game;
class board;

class NetworkMessageManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkMessageManager(Tetris_Game* aGame,QObject* parent = nullptr);
    ~NetworkMessageManager();

    void setInputParameters(quint16 port);
    void setOutputParameters(const QString& ip, quint16 port);

    void startListeningToPrevious(); // Rozpocznij nasluchiwanie od poprzedniego gracza
    void startConnectingToNext();    // Polacz z nastepnym graczem
    void stopWorking();

    void proceedMessageFromPrevious(const NetworkMessage& message);
    void sendMessageToNext(const NetworkMessage message);

    void setLocalPlayer(const Player& localPlayer);
    Player getLocalPlayer();

    bool isEveryOneListenMe();
    bool isListenOlnyOtherPlayer();
    void waitForMessage(const NetworkMessage& message);
    void waitForMessageFinish(const NetworkMessage& message);
    void setPriorityMessageBeforeSendingListenOlnyMeMessage(const NetworkMessage& message);

signals:
    void messageProcessed(const NetworkMessage& message);
    void messageReceived(const NetworkMessage& message);
    void messageFinished(const NetworkMessage& messsage);
private slots:
    void onReadyRead();

private:
    void serveDeleteRowMessage(const NetworkMessage& message);
    void serveCanRowBeDeletedMessage(const NetworkMessage& message);
    void serveMoveAllDownMessage(const NetworkMessage& message);
    void serveListenOnlyMeMessage(const NetworkMessage& message);
    void serveStopListenOnlyMeMessage(const NetworkMessage& message);

    NetworkMessage lastMessage;
    NetworkMessage lastFinishMessage;

    Player* priorityPlayer;
    bool isListenOnlyOnePlayer;
    QTime* priorityMessageSendTime;
    bool everyOneListenMe;
    bool localPlayerSentListenOnlyMeMessage;

    Player localPlayer;
    QTcpServer* inputServer;  // Server for listening for incoming connections
    QTcpSocket* inputSocket;  // Connection with previous player
    QTcpSocket* outputSocket; // Connection with next player
    QString nextNeighborIP;
    quint16 nextNeighborPort;
    quint16 previousNeighborPort;
    board* game_board;
    Tetris_Game* game;
};

#endif // NETWORKMESSAGEMANAGER_H
