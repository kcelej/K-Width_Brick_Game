#include "NetworkMessage.h"
#include <QIODevice>
#include <QDataStream>

NetworkMessage::NetworkMessage(const Player& sender, const QString& text, const QTime& sendTime, NetworkMessageType type)
    : sender(sender), text(text), sendTime(sendTime), typeOfMessage(type) {
}

Player NetworkMessage::getSender() const { return sender; }
QString NetworkMessage::getText() const { return text; }
QTime NetworkMessage::getSendTime() const { return sendTime; }
NetworkMessage::NetworkMessageType NetworkMessage::getType() const { return typeOfMessage; }

QByteArray NetworkMessage::toByteArray() const {
    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);

    // Serializacja danych
    out << static_cast<int>(typeOfMessage);  // Serializowanie typu wiadomoœci
    out << sender.getIpAddress();
    out << sender.getId();
    out << text;
    out << sendTime;

    return byteArray;
}

NetworkMessage NetworkMessage::fromByteArray(const QByteArray& data) {
    QDataStream in(data);

    // Dane tymczasowe do odczytu
    int messageTypeInt;
    QString senderIp;
    int senderId;
    QString messageText;
    QTime messageSendTime;

    // Deserializacja danych
    in >> messageTypeInt;  // Deserializacja typu wiadomoœci
    in >> senderIp;
    in >> senderId;
    in >> messageText;
    in >> messageSendTime;

    // Przekszta³cenie int na typ NetworkMessageType
    NetworkMessageType messageType = static_cast<NetworkMessageType>(messageTypeInt);

    // Utwórz obiekt Player i NetworkMessage
    Player sender(senderIp, senderId);
    return NetworkMessage(sender, messageText, messageSendTime, messageType);
}
