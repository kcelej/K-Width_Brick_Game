#include "NetworkMessage.h"
#include <QIODevice>
#include <QDataStream>

NetworkMessage::NetworkMessage(const Player& sender, const QString& text, const QTime& sendTime, NetworkMessageType type)
    : sender(sender), text(text), sendTime(sendTime), typeOfMessage(type) {
}

NetworkMessage::NetworkMessage(const NetworkMessage& toCopy) {
    this->sender = toCopy.sender;
    this->sendTime = toCopy.sendTime;
    this->text = toCopy.text;
    this->typeOfMessage = toCopy.typeOfMessage;
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

NetworkMessage::CanRowBeDeletedMessageInfo::CanRowBeDeletedMessageInfo(int numberOfPlayersVal, int numberOfRowVal): playersRowState(numberOfPlayersVal,false) {
    numberOfPlayers = numberOfPlayersVal;
    numberOfRow = numberOfRowVal;
}

NetworkMessage::CanRowBeDeletedMessageInfo NetworkMessage::CanRowBeDeletedMessageInfo::fromString(QString str) {
    QStringList strList = str.split(',');
    int numberOfPlayers = strList.at(0).toInt();
    int numberOfRow = strList.at(1).toInt();
    std::vector<bool> playersRowState(numberOfPlayers,false);
    for (int i = 0; i < numberOfPlayers; ++i) {
        playersRowState[i] = (strList.at(i + 2) == "0") ? false : true;
    }
    CanRowBeDeletedMessageInfo messageInfo(numberOfPlayers, numberOfRow);
    messageInfo.playersRowState = playersRowState;
    return messageInfo;
}
QString NetworkMessage::CanRowBeDeletedMessageInfo::toString() {
    QString str = QString("%1,%2").arg(numberOfPlayers).arg(numberOfRow);
    for (int i = 0; i < numberOfPlayers; ++i) {
        if (playersRowState[i] == true) {
            str.append(",1");
        }
        else {
            str.append(",0");
        }
    }
    return str;
}

NetworkMessage::CanRowBeDeletedMessageInfo& NetworkMessage::CanRowBeDeletedMessageInfo::operator=(const CanRowBeDeletedMessageInfo& rv) {
    numberOfPlayers = rv.numberOfPlayers;
    numberOfRow = rv.numberOfRow;
    playersRowState = rv.playersRowState;
    return *this;
}
//text nie jest porownwywane
bool NetworkMessage::operator==(const NetworkMessage& rv) const {
    return sender.getId() == rv.sender.getId() && sendTime == rv.sendTime && typeOfMessage == rv.typeOfMessage;
}

NetworkMessage& NetworkMessage::operator=(const NetworkMessage& rv) {
    this->sender = rv.sender;
    this->sendTime = rv.sendTime;
    this->text = rv.text;
    this->typeOfMessage = rv.typeOfMessage;
    return *this;
}