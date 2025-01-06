#ifndef NETWORKMESSAGEFACTORY_H
#define NETWORKMESSAGEFACTORY_H

#include "NetworkMessage.h"

class NetworkMessageFactory {
public:
    static NetworkMessage createDeleteRowMessage(const Player& sender, int rowNumber);
    static NetworkMessage createCanRowBeDeletedMessage(const Player& sender, int rowNumber);
};

#endif // NETWORKMESSAGEFACTORY_H
