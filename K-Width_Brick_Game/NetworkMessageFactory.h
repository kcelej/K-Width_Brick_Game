#ifndef NETWORKMESSAGEFACTORY_H
#define NETWORKMESSAGEFACTORY_H

#include "NetworkMessage.h"

class NetworkMessageFactory {
public:
    static NetworkMessage createRowDeletedMessage(const Player& sender);
    static NetworkMessage createCanRowBeDeletedMessage(const Player& sender);
};

#endif // NETWORKMESSAGEFACTORY_H
