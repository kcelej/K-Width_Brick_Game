#pragma once

#include "Player.h"
#include "NetworkMessageManager.h"
#include "Tetris_Game.h"
#include "NetworkUtils.h"
#include <QString>

class NetworkGame
{
private:
	Player localPlayer;
	NetworkMessageManager networkMessageManager;
	//Tetris_Game tetrisGame;
public:
	NetworkGame();
	~NetworkGame();
	void startGame();
	void configNetworkNeighbors(QString nextNeighborIP, quint16 nextNeighborPort, quint16 previousNeighborPort);
	void setUserId(int id);
	void startListening();
	void connectToNext();
  
};

