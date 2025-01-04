//#ifndef NETWORKMESSAGEMANAGER_H
//#define NETWORKMESSAGEMANAGER_H
//
//#include "NetworkMessage.h"
//#include <QTcpSocket>
//#include <QObject>
//
//class NetworkMessageManager : public QObject {
//    Q_OBJECT
//
//public:
//    explicit NetworkMessageManager(QObject* parent = nullptr);
//
//    void setInputParameters(quint16 port);
//    void setOutputParameters(const QString& ip, quint16 port);
//
//    void startWorking();
//    void stopWorking();
//
//    void proceedMessageFromPrevious(const NetworkMessage& message);
//    void sendMessageToNext(const NetworkMessage& message);
//
//    void setLocalPlayer(const Player& localPlayer);
//
//signals:
//    void messageProcessed(const NetworkMessage& message);
//
//private slots:
//    void onReadyRead();
//
//private:
//
//    void serveDeleteRowMessage(const NetworkMessage& message);
//    void serveCanRowBeDeletedMessage(const NetworkMessage& message);
//
//    Player localPlayer;
//    QTcpSocket* inputSocket;
//    QTcpSocket* outputSocket;
//    QString nextNeighborIP;
//    quint16 nextNeighborPort;
//    quint16 previousNeighborPort;
//};
//
//#endif // NETWORKMESSAGEMANAGER_H
#ifndef NETWORKMESSAGEMANAGER_H
#define NETWORKMESSAGEMANAGER_H

#include "NetworkMessage.h"
#include "Player.h"
#include <QTcpSocket>
#include <QTcpServer>
#include <QObject>

class NetworkMessageManager : public QObject {
    Q_OBJECT

public:
    explicit NetworkMessageManager(QObject* parent = nullptr);
    ~NetworkMessageManager();

    void setInputParameters(quint16 port);
    void setOutputParameters(const QString& ip, quint16 port);

    void startListeningToPrevious(); // Rozpocznij nasłuchiwanie od poprzedniego gracza
    void startConnectingToNext();    // Połącz z następnym graczem
    void stopWorking();

    void proceedMessageFromPrevious(const NetworkMessage& message);
    void sendMessageToNext(const NetworkMessage& message);

    void setLocalPlayer(const Player& localPlayer);

signals:
    void messageProcessed(const NetworkMessage& message);

private slots:
    void onReadyRead();

private:
    void serveDeleteRowMessage(const NetworkMessage& message);
    void serveCanRowBeDeletedMessage(const NetworkMessage& message);

    Player localPlayer;
    QTcpServer* inputServer;  // Serwer do nasłuchiwania na połączenia przychodzące
    QTcpSocket* inputSocket;  // Połączenie z poprzednim graczem
    QTcpSocket* outputSocket; // Połączenie z następnym graczem
    QString nextNeighborIP;
    quint16 nextNeighborPort;
    quint16 previousNeighborPort;
};

#endif // NETWORKMESSAGEMANAGER_H
