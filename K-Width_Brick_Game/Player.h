#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player {
public:
    Player(const QString& ip = "", int id = 0);
    ~Player(); // Destruktor

    QString getIpAddress() const;
    int getId() const;

    void setIpAddress(const QString& ip);
    void setId(int id);
    Player& operator=(const Player& rv);
    Player(const Player& toCopy);

private:
    QString ipAddress;
    int playerId;
};

#endif // PLAYER_H
