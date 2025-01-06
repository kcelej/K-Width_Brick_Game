#include "NetworkMessageFactory.h"
#include <QTime>
#include "Defines.h"

NetworkMessage NetworkMessageFactory::createDeleteRowMessage(const Player& sender,int rowNumber) {
    QString text = QString("%1").arg(rowNumber);
    return NetworkMessage(sender, text, QTime::currentTime(),NetworkMessage::deleteRow);
}

NetworkMessage NetworkMessageFactory::createCanRowBeDeletedMessage(const Player& sender, int rowNumber) {
    //numer wiersza, liczba graczy n,wartosc u gracza 1, wartosc u gracza 2,..., wartosc u gracza n
    QString text = QString("%1,%2").arg(rowNumber).arg(NUMBER_OF_PLAYERS);
    for (int i = 0; i < NUMBER_OF_PLAYERS; ++i) {
        text.append(",0");
    }
    return NetworkMessage(sender, text, QTime::currentTime(),NetworkMessage::canRowBeDeleted);
}
