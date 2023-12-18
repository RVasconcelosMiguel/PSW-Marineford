#include "Map.h"
#include "Gui.h"
//#include "Ship.h"

extern GUI *UI;

Map::Map(){

}

QList<Ship *> Map::getShips(){
    return ships;
}

void Map::placeShips(int x, int y, int numRow){
    for(size_t i=0; i<numRow; i++){
        createShipRow(x, y+i*30, 10);
    }
}

void Map::placeMyMap(int x, int y){
    for(size_t i=0; i<10; i++){
        for(size_t j=0; j<10; j++){
            Ship * ship = new Ship(1, 1);
            ship->isClickable = false;
            ship->setPos(x+j*30, y+i*30);
            if(binmap[i][j]==0){
                ship->setOwner(QString("MAP"));
            }
            else{
                ship->setOwner(QString("PLAYER"));
            }
            UI->scene->addItem(ship);
        }
    }
}

void Map::placeEnemyMap(int x, int y){
    for(size_t i=0; i<10; i++){
        for(size_t j=0; j<10; j++){
            Ship * ship = new Ship(1, 1);
            ship->isClickable = false;
            ship->setPos(x+j*30, y+i*30);
            if(enemyBinMap[i][j]==0){
                ship->setOwner(QString("MAP"));
            }
            else{
                ship->setOwner(QString("PLAYER"));
            }
            UI->scene->addItem(ship);
        }
    }

    for(size_t i=0; i<10; i++){
        for(size_t j=0; j<10; j++){
            Ship * ship = new Ship(1, 1);
            ship->isClickable = true;
            ship->setPos(x+j*30, y+i*30);
            ship->setOwner("REMOVABLE");
            removableShips.append(ship);
            UI->scene->addItem(ship);
        }
    }
}

void Map::createShipRow(int x, int y, int numCol){
    for(size_t i=0; i<numCol; i++){
        Ship * ship = new Ship(1, 1);
        ship->setIsPlaced(true);
        ship->setOwner(QString("MAP"));
        ship->setPos(x+i*30,y);
        UI->scene->addItem(ship);
        ships.append(ship);
    }
}
