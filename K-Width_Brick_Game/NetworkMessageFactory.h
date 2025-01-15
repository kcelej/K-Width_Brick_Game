#ifndef NETWORKMESSAGEFACTORY_H
#define NETWORKMESSAGEFACTORY_H

#include "NetworkMessage.h"

class NetworkMessageFactory {
public:
    static NetworkMessage createDeleteRowMessage(const Player& sender, int rowNumber);
    static NetworkMessage createMoveAllDownMessage(const Player& sender, int rowNumber);
    static NetworkMessage createCanRowBeDeletedMessage(const Player& sender, int rowNumber, bool senderRowState);
    static NetworkMessage createListenOnlyMeMessage(const Player& sender);
    static NetworkMessage createStopListenOnlyMeMessage(const Player& sender);
};

#endif // NETWORKMESSAGEFACTORY_H
