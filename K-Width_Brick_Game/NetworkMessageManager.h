#ifndef NETWORKMESSAGEMANAGER_H
#define NETWORKMESSAGEMANAGER_H

#include "NetworkMessage.h"
#include "Player.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class board;

class NetworkMessageManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkMessageManager(board* boardGame,QObject* parent = nullptr);
    ~NetworkMessageManager();

    void setInputParameters(quint16 port);
    void setOutputParameters(const QString& ip, quint16 port);

    void startListeningToPrevious(); // Rozpocznij nasluchiwanie od poprzedniego gracza
    void startConnectingToNext();    // Polacz z nastepnym graczem
    void stopWorking();

    void proceedMessageFromPrevious(const NetworkMessage& message);
    void sendMessageToNext(const NetworkMessage& message);

    void setLocalPlayer(const Player& localPlayer);
    Player getLocalPlayer();
signals:
    void messageProcessed(const NetworkMessage& message);

private slots:
    void onReadyRead();

private:
    void serveDeleteRowMessage(const NetworkMessage& message);
    void serveCanRowBeDeletedMessage(const NetworkMessage& message);

    Player localPlayer;
    QTcpServer* inputServer;  // Serwer do nasluchiwania na polaczenia przychodzace
    QTcpSocket* inputSocket;  // Polaczenie z poprzednim graczem
    QTcpSocket* outputSocket; // Polaczenie z nastepnym graczem
    QString nextNeighborIP;
    quint16 nextNeighborPort;
    quint16 previousNeighborPort;
    board* game_board;
};

#endif // NETWORKMESSAGEMANAGER_H
