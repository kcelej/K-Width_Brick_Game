#include "NetworkMessageFactory.h"
#include <QTime>
#include "Defines.h"
#include "PlayerNetworkConfig.h"

NetworkMessage NetworkMessageFactory::createDeleteRowMessage(const Player& sender,int rowNumber) {
    QString text = QString("%1").arg(rowNumber);
    return NetworkMessage(sender, text, QTime::currentTime(),NetworkMessage::deleteRow);
}

NetworkMessage NetworkMessageFactory::createMoveAllDownMessage(const Player& sender, int rowNumber) {
    QString text = QString("%1").arg(rowNumber);
    return NetworkMessage(sender, text, QTime::currentTime(), NetworkMessage::moveAllDown);
}

NetworkMessage NetworkMessageFactory::createCanRowBeDeletedMessage(const Player& sender, int rowNumber, bool senderRowState) {
    //numer wiersza, liczba graczy n,wartosc u gracza 1, wartosc u gracza 2,..., wartosc u gracza n
    PlayerNetworkConfig& config = PlayerNetworkConfig::getInstance();
    qDebug() << "Player count:" << config.PLAYER_COUNT;
    NetworkMessage::CanRowBeDeletedMessageInfo messageInfo(config.PLAYER_COUNT, rowNumber);
    
    messageInfo.playersRowState[sender.getId() - 1] = senderRowState;
    return NetworkMessage(sender, messageInfo.toString() , QTime::currentTime(), NetworkMessage::canRowBeDeleted);
}

NetworkMessage NetworkMessageFactory::createListenOnlyMeMessage(const Player& sender) {
    return NetworkMessage(sender,"", QTime::currentTime(), NetworkMessage::listenOnlyMe);
}
NetworkMessage NetworkMessageFactory::createStopListenOnlyMeMessage(const Player& sender) {
    return NetworkMessage(sender, "", QTime::currentTime(), NetworkMessage::stopListenOnlyMe);
}