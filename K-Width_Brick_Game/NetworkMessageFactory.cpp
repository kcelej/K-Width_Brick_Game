#include "NetworkMessageFactory.h"
#include <QTime>

NetworkMessage NetworkMessageFactory::createRowDeletedMessage(const Player& sender) {
    return NetworkMessage(sender, "RowDeleted", QTime::currentTime(),NetworkMessage::deleteRow);//todo
}

NetworkMessage NetworkMessageFactory::createCanRowBeDeletedMessage(const Player& sender) {
    return NetworkMessage(sender, "CanRowBeDeleted", QTime::currentTime(),NetworkMessage::canRowBeDeleted);//todo
}
