#pragma once
#ifndef PLAYERNETWORKCONFIG_H
#define PLAYERNETWORKCONFIG_H

#include <QString>

class PlayerNetworkConfig {
public:
    static PlayerNetworkConfig& getInstance() {
        static PlayerNetworkConfig instance;
        return instance;
    }
    
    int PLAYER_COUNT;

    int listenPort;
    int sendPort;
    QString sendIp;
    int playerId;

    int hostport;

    int getListenPort() const { return listenPort; }
    int getSendPort() const { return sendPort; }
    QString getSendIp() const { return sendIp; }

    /*static PlayerNetworkConfig& instance() {
        static PlayerNetworkConfig config;
        return config;
    }*/

private:
    PlayerNetworkConfig() = default;
};




#endif // PLAYERNETWORKCONFIG_H