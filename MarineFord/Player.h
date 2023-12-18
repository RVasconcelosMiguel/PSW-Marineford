#ifndef PLAYER_H
#define PLAYER_H
#include <QObject>

class Player
{
public:
    Player();
    int receivedMap[10][10] = {};
    int hasRec = 0;
    bool hasSend = false;
    QString chatMessage;
};

#endif // PLAYER_H
