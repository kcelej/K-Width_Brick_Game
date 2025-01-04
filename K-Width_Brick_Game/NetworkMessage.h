#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

#include "Player.h"
#include <QString>
#include <QTime>
#include <QByteArray> 
#include <QDataStream> 



class NetworkMessage {
public:
    enum NetworkMessageType {
        deleteRow,
        canRowBeDeleted
    };

    NetworkMessage(const Player& sender, const QString& text, const QTime& sendTime, NetworkMessageType type);

    Player getSender() const;
    QString getText() const;
    QTime getSendTime() const;
    NetworkMessageType getType() const;
    QByteArray toByteArray() const; // Serializacja
    static NetworkMessage fromByteArray(const QByteArray& data); // Deserializacja

private:
    Player sender;
    NetworkMessageType typeOfMessage;
    QString text;
    QTime sendTime;
};

#endif // NETWORKMESSAGE_H
