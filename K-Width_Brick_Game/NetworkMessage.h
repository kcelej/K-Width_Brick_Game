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
        canRowBeDeleted,
        moveAllDown,
        listenOnlyMe,
        stopListenOnlyMe
    };

    struct CanRowBeDeletedMessageInfo {
    public:
        CanRowBeDeletedMessageInfo(int numberOfPlayersVal, int numberOfRowVal);

        static CanRowBeDeletedMessageInfo fromString(QString str);
        QString toString();

        CanRowBeDeletedMessageInfo& operator=(const CanRowBeDeletedMessageInfo & rv);

        int numberOfPlayers;
        std::vector<bool> playersRowState;
        int numberOfRow;
    };

    NetworkMessage(const Player& sender, const QString& text, const QTime& sendTime, NetworkMessageType type);
    NetworkMessage() = default;
    NetworkMessage(const NetworkMessage& toCopy);

    Player getSender() const;
    QString getText() const;
    QTime getSendTime() const;
    NetworkMessageType getType() const;
    QByteArray toByteArray() const; // Serialization
    static NetworkMessage fromByteArray(const QByteArray& data); // Deserialization

    bool operator==(const NetworkMessage& rv) const;
    NetworkMessage& operator=(const NetworkMessage& rv);

private:
    Player sender;
    NetworkMessageType typeOfMessage;
    QString text;
    QTime sendTime;
};


#endif // NETWORKMESSAGE_H
