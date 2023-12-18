#ifndef MAP_H
#define MAP_H

#include <QList>
#include <QGraphicsRectItem>
#include <QVector>
#include "Ship.h"

class Map{
public:
    //constructor
    Map();

    //getters
    QList<Ship*> getShips();

    //setters
    //void setSquares();

    //public methods
    void placeShips(int x, int y, int numRow);
    int binmap[10][10] = {};
    int enemyBinMap[10][10] = {};

    void placeMyMap(int x, int y); //xy pos
    void placeEnemyMap(int x, int y);  //xypos

    QList<Ship*> removableShips;

private:
    void createShipRow(int x, int y, int numCol);
    QList<Ship*> ships;
};

#endif // MAP_H
