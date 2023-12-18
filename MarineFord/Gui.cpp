#include "Gui.h"
#include "Map.h"
#include "Ship.h"
#include "Server.h"
#include "Client.h"

#include <QGraphicsTextItem>
#include <QBrush>
#include <QFont>
#include <QPen>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QDebug>
#include <QEventLoop>
#include <QMediaPlayer>
#include <QUrl>
#include <QAudioOutput>
#include <QUrl>
#include <QScrollBar>
#include <QProcess>
#include <QApplication>
#include <iostream>

//GUI
GUI::GUI(QWidget *parent){
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1000,500);
    setScene(scene);
    shipToPlace=NULL;
    connect(&server, &QTcpServer::newConnection, this, &GUI::newConnection);
    connect(&client, &Client::clientConnected, this, &GUI::onClientConnected);
    connect(&client, &Client::dataReceived, this, &GUI::clientCheck);
    connect(&server, &Server::dataReceived, this, &GUI::serverCheck);
    connect(&server, &Server::chatDataReceived, this, &GUI::updateEnemyChat);
    connect(&client, &Client::chatDataReceived, this, &GUI::updateEnemyChat);
    connect(&client, &Client::clientDisconnected, this, &GUI::restartApplication);
    connect(&server, &Server::serverDisconnect, this, &GUI::restartApplication);

    background = new QMediaPlayer(); // Inicialização do QMediaPlayer
    audiooutput = new QAudioOutput();
    audiooutput->setVolume(0.01);
    background->setAudioOutput(audiooutput);
    background->setSource(QUrl("qrc:/Sounds/background.mp3"));
    background->play();
    background->setLoops(-1);

}
//GUI
void GUI::setBackground(QString source)
{
    QPixmap backgroundImage(source);
    backgroundImage = backgroundImage.scaled(1000, 500, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    QGraphicsPixmapItem *background = new QGraphicsPixmapItem(backgroundImage);
    scene->addItem(background);
}
//GAME
void GUI::createShips(int x, int y, int size, int numid)
{
    Ship * ship = new Ship(size, numid); //int tamanho
    ship->setOwner(QString("PLAYER"));
    ship->setIsPlaced(false);
    playerShips.append(ship);
    //    std::string message = "The value of size is: ";
    //    std::cout << message << playerShips.size() << std::endl;
//GUI
    ship->setPos(x,y);
    scene->addItem(ship);
}
//GUI
void GUI::pickUpShip(Ship *ship){
    if(shipToPlace==NULL){
        shipToPlace=ship;
        originalPos=ship->pos();
        return;
    }
}

void GUI::placeShip(Ship *mapShipToReplace){

    if(mapShipToReplace->getOwner() == "PLAYER"){
        handleInvalidPos();
        return;
    }

    int posToVerify;
    int xposToVerify;
    int yposToVerify;
    for(size_t i=0;i<map->getShips().size();i++){
        if(map->getShips().at(i)==mapShipToReplace){
            if(shipToPlace->direction==QString("horizontal") && (i % 10)+ shipToPlace->getSize() > 10){
                handleInvalidPos();
                return;
            }
            if(shipToPlace->direction==QString("vertical") && ((i - i % 10) / 10) + shipToPlace->getSize() > 10){
                handleInvalidPos();
                return;
            }
            for(int s = 0 ; s<shipToPlace->getSize(); s++){
                if(shipToPlace->direction==QString("horizontal")){
                    posToVerify=i+s;
                    xposToVerify = (posToVerify-posToVerify%10)/10;
                    yposToVerify = posToVerify%10;
                    if(invalidPos[xposToVerify][yposToVerify]==1){
                        handleInvalidPos();
                        return;
                    }
                }
                if(shipToPlace->direction==QString("vertical")){
                    posToVerify=i+10*s;
                    xposToVerify = (posToVerify-posToVerify%10)/10;
                    yposToVerify = posToVerify%10;
                    if(invalidPos[xposToVerify][yposToVerify]==1){
                        handleInvalidPos();
                        return;
                    }
                }
            }
        }
    }


    for (size_t i = 0; i < map->getShips().size(); i++) {
        if (map->getShips().at(i) == mapShipToReplace) {
            int fno = (i - i % 10) / 10;
            int sno = i % 10;
            int sn;
            int fn;
            for (int s = 0; s < shipToPlace->getSize(); s++) {
                if (shipToPlace->direction == QString("horizontal")) {
                    sn = sno + s;
                    fn = fno;  // Initialize fn based on the loop variable s
                    if (fn - 1 >= 0 && sn - 1 >= 0)
                        invalidPos[fn - 1][sn - 1] = 1;
                    if (fn - 1 >= 0)
                        invalidPos[fn - 1][sn] = 1;
                    if (fn - 1 >= 0 && sn + 1 <= 9)
                        invalidPos[fn - 1][sn + 1] = 1;
                    if (sn - 1 >= 0)
                        invalidPos[fn][sn - 1] = 1;
                    invalidPos[fn][sn] = 1;
                    if (sn + 1 <= 9)
                        invalidPos[fn][sn + 1] = 1;
                    if (fn + 1 <= 9 && sn - 1 >=0)
                        invalidPos[fn + 1][sn - 1] = 1;
                    if (fn + 1 <= 9)
                        invalidPos[fn + 1][sn] = 1;
                    if (fn + 1 <= 9 && sn + 1 <= 9)
                        invalidPos[fn + 1][sn + 1] = 1;
                }
                if (shipToPlace->direction == QString("vertical")) {
                    fn = fno + s;
                    sn = sno;  // Initialize sn based on the loop variable s
                    if (fn - 1 >= 0 && sn - 1 >= 0)
                        invalidPos[fn - 1][sn - 1] = 1;
                    if (fn - 1 >= 0)
                        invalidPos[fn - 1][sn] = 1;
                    if (fn - 1 >= 0 && sn + 1 <= 9)
                        invalidPos[fn - 1][sn + 1] = 1;
                    if (sn - 1 >= 0)
                        invalidPos[fn][sn - 1] = 1;
                    invalidPos[fn][sn] = 1;
                    if (sn + 1 <= 9)
                        invalidPos[fn][sn + 1] = 1;
                    if (fn + 1 <= 9 && sn - 1 >=0)
                        invalidPos[fn + 1][sn - 1] = 1;
                    if (fn + 1 <= 9)
                        invalidPos[fn + 1][sn] = 1;
                    if (fn + 1 <= 9 && sn + 1 <= 9)
                        invalidPos[fn + 1][sn + 1] = 1;
                }
            }
        }
    }



    //discover position of ship to replace and storing in in binmap
    for(size_t i=0;i<map->getShips().size();i++){
        if(map->getShips().at(i)==mapShipToReplace){
            std::cout << i << std::endl;
            int fn= (i-i%10)/10;
            int sn=i%10;
            for(int j=0;j<shipToPlace->getSize();j++){
                if(shipToPlace->direction==QString("horizontal")){
                    map->binmap[fn][sn+j]=shipToPlace->id;
                }
                if(shipToPlace->direction==QString("vertical")){
                    map->binmap[fn+j][sn]=shipToPlace->id;
                }
            }
        }
    }

    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            std::cout << map->binmap[i][j];
        }
        std::cout << std::endl;
    }
    //changing the ship from map to player's
    shipToPlace->setPos(mapShipToReplace->pos());
    map->getShips().removeAll(mapShipToReplace);
    map->getShips().append(shipToPlace);
    scene->removeItem(mapShipToReplace);
    shipToPlace->setIsPlaced(true);
    scene->addItem(shipToPlace);
    removeFromPlayersShips(shipToPlace);
    shipToPlace=NULL;


    qDebug() <<"barcos"<< playerShips.size();
    if(playerShips.size()==0){
        Button* readyButton = new Button(18, QString("Ready"), Qt::green, Qt::darkGreen, 210, 50);
        readyButton->setPos(200,360);
        if(imServer){
            connect(readyButton,SIGNAL(clicked()),this,SLOT(serversendBinMapMenu()));
        }else{
            connect(readyButton,SIGNAL(clicked()),this,SLOT(clientsendBinMapMenu()));
        }
        connect(readyButton, &Button::clicked, [=]() {
            scene->removeItem(readyButton);

        });
        scene->addItem(readyButton);
    }


}

void GUI::removeFromPlayersShips(Ship *ship){
    playerShips.removeAll(ship);
}

//GUI
void GUI::gameMenu(){
    inGame = true;
    setBackground(":/Images/game_background_2.png");
    int pos_x = (this->width()/2);
    setMenuText("Marineford", 45, pos_x, 30, true);
    map->placeMyMap(50, 100);
    map->placeEnemyMap(400, 100);
    Button* send = new Button(10,QString("Send"), Qt::lightGray, Qt::gray, 50, 20);
    send->setPos(920, 350);
    connect(send,SIGNAL(clicked()),this,SLOT(sendMsg()));
    connect(send,SIGNAL(clicked()),this,SLOT(updateChat()));
    scene->addItem(send);
    Button* backButton = new Button(18, QString("Quit"), Qt::red, Qt::darkRed, 80, 40);
    backButton->setPos(20,20);
    connect(backButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(backButton);

    setText("KILL", 10, 355, 95, "white");
    setText("COUNT", 10, 352, 110, "white");


    if(imServer){
        titleText = new QGraphicsTextItem("Your Turn");
        titleText->setDefaultTextColor("red");
        QFont titleFont("Courier",25, QFont::Bold);
        titleText->setFont(titleFont);
        titleText->setPos(460, 400);
        scene->addItem(titleText);
    }
    else{
        titleText = new QGraphicsTextItem("Enemy's Turn");
        titleText->setDefaultTextColor("red");
        QFont titleFont("Courier",25, QFont::Bold);
        titleText->setFont(titleFont);
        titleText->setPos(75, 400);
        scene->addItem(titleText);
    }
    setText("Chat", 15, 815, 120, "white");
    nameLineEdit = new QLineEdit();
    QGraphicsProxyWidget* proxy = scene->addWidget(nameLineEdit);
    nameLineEdit->setPlaceholderText(QString("Write your message here"));
    proxy->setPos(715,350);
    connect(nameLineEdit,&QLineEdit::textChanged,this,&GUI::readMsg);

    chatTextEdit = new QTextEdit();
    chatTextEdit->setReadOnly(true); // Defined as read only
    chatTextEdit->setStyleSheet("background-color: rgba(255, 255, 255, 150); QScrollBar:vertical { width: 0; }");


    // Add the QTextEdit to the scene using QGraphicsProxyWidget
    QGraphicsProxyWidget* chatProxy = scene->addWidget(chatTextEdit);
    chatProxy->setPos(715, 150); // Position superimposed to the QLineEdit

}

//GUI
void GUI::gameOverWin()
{
    scene->clear();

    setBackground(":/Images/game_background.png");
    setText("GAME OVER YOU WON!", 45, 175, 120, "green");


    Button* joinButton = new Button(18, QString("Back to Main menu"), Qt::green, Qt::darkGreen, 350, 50);


    int xquitPos = 20;
    int yPos = 250;
    int yquitpos = 20;

    joinButton->setPos(325,yPos);

    connect(joinButton, SIGNAL(clicked()), this, SLOT(endGame()));

    scene->addItem(joinButton);

    victory= new QMediaPlayer();
    victoryoutput= new QAudioOutput();
    victory->setAudioOutput(victoryoutput);
    victoryoutput->setVolume(0.2);
    victory->setSource(QUrl("qrc:/Sounds/victory.mp3"));
    victory->play();
}

//GUI
void GUI::gameOverLose()
{
    scene->clear();

    setBackground(":/Images/game_background.png");
    setText("GAME OVER YOU LOST.",45, 175, 120, "red");


    Button* joinButton = new Button(18, QString("Back to Main menu"), Qt::red, Qt::darkRed, 350, 50);


    int xquitPos = 20;
    int yPos = 250;
    int yquitpos = 20;

    joinButton->setPos(325,yPos);

    connect(joinButton, SIGNAL(clicked()), this, SLOT(endGame()));

    scene->addItem(joinButton);

    defeat= new QMediaPlayer();
    defeatoutput= new QAudioOutput();
    defeat->setAudioOutput(defeatoutput);
    defeatoutput->setVolume(0.2);
    defeat->setSource(QUrl("qrc:/Sounds/defeat.mp3"));
    defeat->play();




}

void GUI::handleInvalidPos(){
    if(shipToPlace!=NULL){
        if(shipToPlace->direction==QString("vertical")){
            shipToPlace->rotateShip();
        }
        //shipToPlace->rotateShip();
        shipToPlace->setPos(originalPos);
        shipToPlace=NULL;
    }
}
//GUI
void GUI::shipDown(){
    QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/Images/scull.png"));
    scene->addItem(pixmapItem);

    int myInt = 10-shipsDown;

    // Convert int to QString
    QString myString = QString::number(myInt);

    scene->removeItem(kills);
    kills = new QGraphicsTextItem(myString);

    QFont titleFont("Courier",10, QFont::Bold);
    kills->setFont(titleFont);

    // Set text color
    QColor textColor("white");
    kills->setDefaultTextColor(textColor);


    kills->setPos(350, 400);

    scene->addItem(kills);

    setText("LEFT", 10, 360, 400, "white");

    pixmapItem->setPos(360, 100 +shipsDown*30);
}

//create a class for chat ?
void GUI::sendMsg()
{
    if(imServer){
        QString chatMessage = "CHAT:" + msg;
        QByteArray byteArray = chatMessage.toUtf8();

        char* writableData = new char[byteArray.size() + 1];
        std::copy(byteArray.constBegin(), byteArray.constEnd(), writableData);
        writableData[byteArray.size()] = '\0';

        server.write(writableData);
        delete[] writableData;
    }
    else{
        QString chatMessage = "CHAT:" + msg;
        QByteArray byteArray = chatMessage.toUtf8();

        char* writableData = new char[byteArray.size() + 1];
        std::copy(byteArray.constBegin(), byteArray.constEnd(), writableData);
        writableData[byteArray.size()] = '\0';

        client.sendData(writableData);
        delete[] writableData;
    }
}

void GUI::updateChat()
{
    chatMsg+=QString("YOU: ") +msg+"\n";
    chatTextEdit->setText(chatMsg);

    updateChatBox();
    nameLineEdit->clear();
}

void GUI::updateEnemyChat()
{
    if(imServer){
        chatMsg+=QString("ENEMY: ")+server.chatMessage+"\n";
        chatTextEdit->setText(chatMsg);

        updateChatBox();
    }
    else{
        chatMsg+=QString("ENEMY: ")+client.chatMessage+"\n";
        chatTextEdit->setText(chatMsg);

        updateChatBox();
    }
}

void GUI::updateChatBox() {
    // Update the QTextEdit with the up to date value of 'msg'
    chatTextEdit->setText(chatMsg);
    chatTextEdit->verticalScrollBar()->setValue(chatTextEdit->verticalScrollBar()->maximum());
}

void GUI::restartApplication()
{
    qApp->quit();

    // Restart the application
    QProcess::startDetached(QApplication::applicationFilePath());
}
//GAME
void GUI::discShowMenu()
{
    playerShips.clear();
    client.disconnectFromSever();
   // showMenuSlot();

}
//GAME
void GUI::endGame()
{
    if(imServer){
        server.end();
    }
    else{
        client.disconnectFromSever();
    }
}
//GUI
void GUI::serversendBinMapMenu(){
    //pinting binmap
    setText("Waiting For Oponent...", 22, 120, 360, "white");
    server.hasSend= true;
    for(size_t i=0;i<10;i++){
        for(size_t j=0;j<10;j++){
            std::string strNumber = std::to_string(map->binmap[i][j]);
            server.write(const_cast<char*>(strNumber.c_str()));
        }
    }
    if(server.hasRec>0){
        gameMenu();
    }
}
//GUI
void GUI::clientsendBinMapMenu()
{
    //pinting binmap
    setText("Waiting For Oponent...", 22, 120, 360, "white");
    client.hasSend=true;
    for(size_t i=0;i<10;i++){
        for(size_t j=0;j<10;j++){
            std::string strNumber = std::to_string(map->binmap[i][j]);
            client.sendData(const_cast<char*>(strNumber.c_str()));
        }
    }
    if(client.hasRec>0){
        gameMenu();
    }
}
//GAME
void GUI::clientCheck(){
    qDebug() << "recieved";
    if(client.hasRec==0){
        for(size_t i=0;i<10;i++){
            for(size_t j=0;j<10;j++){
                map->enemyBinMap[i][j]=client.receivedMap[i][j];
                std::cout << map->enemyBinMap[i][j] ;
            }
            std::cout << std::endl;
        }
        for(size_t i=0;i<10;i++){
            for(size_t j=0;j<10;j++){
                std::cout << map->binmap[i][j] ;
            }
            std::cout << std::endl;
        }
    }
    if(client.hasRec>0){
        qDebug()<<  "value on clientcheck" <<map->binmap[client.receivedMap[0][0]][client.receivedMap[0][1]];
        qDebug() << "X:" << client.receivedMap[0][0] << "Y:" << client.receivedMap[0][1];
        if(map->binmap[client.receivedMap[0][0]][client.receivedMap[0][1]]==0){
            qDebug() << "switch";
            imPlaying = !(imPlaying);

            scene->removeItem(titleText);
            titleText = new QGraphicsTextItem("Your Turn");
            titleText->setDefaultTextColor("red");
            QFont titleFont("Courier",25, QFont::Bold);
            titleText->setFont(titleFont);
            titleText->setPos(460, 400);
            scene->addItem(titleText);

            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/Images/fail.png"));
            scene->addItem(pixmapItem);
            pixmapItem->setPos(50+client.receivedMap[0][1]*30, 100+client.receivedMap[0][0]*30);
            water=new QMediaPlayer();
            wateroutput= new QAudioOutput();
            water->setAudioOutput(wateroutput);
            wateroutput->setVolume(0.1);
            water->setSource(QUrl("qrc:/Sounds/agua.mp3"));
            water->play();
        }
        else{

            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/Images/hit.png"));
            scene->addItem(pixmapItem);
            pixmapItem->setPos(50+client.receivedMap[0][1]*30, 100+client.receivedMap[0][0]*30);

            enemyHits++;
            shoot= new QMediaPlayer();
            shootoutput= new QAudioOutput();
            shoot->setAudioOutput(shootoutput);
            shootoutput->setVolume(0.01);
            shoot->setSource(QUrl("qrc:/Sounds/shoot.mp3"));
            shoot->play();
            if(enemyHits>=20){
                gameOverLose();
            }
        }
    }
    client.hasRec++;
    if(client.hasSend && client.hasRec<2){
        gameMenu();
    }
}
//GAME
void GUI::serverCheck()
{
    qDebug() << "recieved";
    if(server.hasRec==0){
        for(size_t i=0;i<10;i++){
            for(size_t j=0;j<10;j++){
                map->enemyBinMap[i][j]=server.receivedMap[i][j];
                std::cout << map->enemyBinMap[i][j] ;
            }
            std::cout << std::endl;
        }

        for(size_t i=0;i<10;i++){
            for(size_t j=0;j<10;j++){
                std::cout << map->binmap[i][j] ;
            }
            std::cout << std::endl;
        }
    }
    if(server.hasRec>0){
        qDebug()<<  "value on servercheck" <<map->binmap[server.receivedMap[0][0]][server.receivedMap[0][1]];
        qDebug() << "X:" << server.receivedMap[0][0] << "Y:" << server.receivedMap[0][1];
        if(map->binmap[server.receivedMap[0][0]][server.receivedMap[0][1]]==0){
            qDebug() << "switch";
            imPlaying = !(imPlaying);

            scene->removeItem(titleText);
            titleText = new QGraphicsTextItem("Your Turn");
            titleText->setDefaultTextColor("red");
            QFont titleFont("Courier",25, QFont::Bold);
            titleText->setFont(titleFont);
            titleText->setPos(460, 400);
            scene->addItem(titleText);

            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/Images/fail.png"));
            scene->addItem(pixmapItem);
            pixmapItem->setPos(50+server.receivedMap[0][1]*30, 100+server.receivedMap[0][0]*30);
            water=new QMediaPlayer();
            wateroutput= new QAudioOutput();
            water->setAudioOutput(wateroutput);
            wateroutput->setVolume(0.1);
            water->setSource(QUrl("qrc:/Sounds/agua.mp3"));
            water->play();
        }
        else{

            QGraphicsPixmapItem *pixmapItem = new QGraphicsPixmapItem(QPixmap(":/Images/hit.png"));
            scene->addItem(pixmapItem);
            pixmapItem->setPos(50+server.receivedMap[0][1]*30, 100+server.receivedMap[0][0]*30);
            shoot= new QMediaPlayer();
            shootoutput= new QAudioOutput();
            shoot->setAudioOutput(shootoutput);
            shootoutput->setVolume(0.01);
            shoot->setSource(QUrl("qrc:/Sounds/shoot.mp3"));
            shoot->play();

            enemyHits++;
            if(enemyHits>=20){
                gameOverLose();
            }
        }
    }
    server.hasRec++;
    if(server.hasSend && server.hasRec<2){
        gameMenu();
    }
}


// readIPslot implementation
void GUI::readIP(const QString &text) {
    qDebug() << "New inserted IP: " << text;
    ip=text;
    emit ipUpdated(text);
}

void GUI::readMsg(const QString &text) {
    qDebug() << "New inserted message: " << text;
    msg=text;
    emit msgUpdated(text);
}
//GUI
void GUI::mouseMoveEvent(QMouseEvent *event){
    //if there is a card to place make it follow the mouse
    if(shipToPlace){
        shipToPlace->setPos(event->pos().x()+5, event->pos().y()+5);
    }

    QGraphicsView::mouseMoveEvent(event);
}
//GUI
void GUI::mousePressEvent(QMouseEvent *event){
    //right click makes ship return to original pos
    if(event->button() == Qt::RightButton){
        if(shipToPlace!=NULL){
            if(shipToPlace->direction==QString("vertical"))
                shipToPlace->rotateShip();

            shipToPlace->setPos(originalPos);
            shipToPlace=NULL;

            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}
//GAME
void GUI::serverEnd()
{
    server.end();


}


//GUI
void GUI::setMenuText(QString title, int size, int x_pos, int y_pos, bool centralized)
{
    QGraphicsTextItem* titleText = new QGraphicsTextItem(title);
    titleText->setDefaultTextColor("white");
    QFont titleFont("Courier",size, QFont::Bold);
    titleText->setFont(titleFont);
    if (centralized){
        titleText->setPos(x_pos-titleText->boundingRect().width()/2, y_pos);
    }
    else {titleText->setPos(x_pos, y_pos);};
    scene->addItem(titleText);
}
//GUI
void GUI::setText(QString title, int size, int x_pos, int y_pos, QString color)
{
    QGraphicsTextItem *textItem = new QGraphicsTextItem(title);
     QFont titleFont("Courier",size, QFont::Bold);
    textItem->setFont(titleFont);

    // Set text color
    QColor textColor(color);
    textItem->setDefaultTextColor(textColor);


    textItem->setPos(x_pos, y_pos);

    scene->addItem(textItem);
}
//GUI
void GUI::showMenu(){

    for(int i = 0; i<10; i++){
        for(int j= 0; j<10; j++){

            std::cout<< "invpos map"<<invalidPos[i][j];
        }
    }

    setBackground(":/Images/game_background.png");
    int pos_x = (this->width()/2);
    setMenuText("MARINEFORD", 45, pos_x, 50, true);

    Button* playButton = new Button(18, QString("New Game"), Qt::red, Qt::darkRed, 200, 50);
    Button* joinButton = new Button(18, QString("Join Game"), Qt::green, Qt::darkGreen, 200, 50);
    Button* quitButton = new Button(18, QString("Quit"), Qt::red, Qt::darkRed, 80, 40);
    int xPos = this->width()/3 - playButton->boundingRect().width()/2;
    int x2Pos = (this->width()/3)*2 - playButton->boundingRect().width()/2;
    int xquitPos = 20;
    int yPos = 250;
    int yquitpos = 20;
    playButton->setPos(xPos,yPos);
    joinButton->setPos(x2Pos,yPos);
    quitButton->setPos(xquitPos, yquitpos);
    connect(playButton,SIGNAL(clicked()),this,SLOT(connectingcreate()));
    connect(joinButton,SIGNAL(clicked()),this,SLOT(showJoinMenu()));
    //connect(playButton,SIGNAL(clicked()),this,SLOT(gameOverWin()));
    //connect(joinButton,SIGNAL(clicked()),this,SLOT(gameOverLose()));
    connect(quitButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(playButton);
    scene->addItem(joinButton);
    scene->addItem(quitButton);
}
//GUI
void GUI::showJoinMenu(){
    scene->clear();
    int xIP = 350, yIP= 200;
    setBackground(":/Images/game_background.png");
    setMenuText("MARINEFORD", 45, this->width()/2, 30, true);
    setMenuText("IP", 18, xIP-80, yIP, false);

    nameLineEdit = new QLineEdit();
    QGraphicsProxyWidget* proxy = scene->addWidget(nameLineEdit);
    nameLineEdit->setPlaceholderText(QString("Insert IP"));
    proxy->setPos(xIP+80-80, yIP+8);
    connect(nameLineEdit,&QLineEdit::textChanged,this,&GUI::readIP);


    Button* backButton = new Button(18, QString("Quit"), Qt::red, Qt::darkRed, 80, 40);
    backButton->setPos(20,20);
    connect(backButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(backButton);

    Button* playButton = new Button(18, QString("Connect"), Qt::green, Qt::darkGreen, 120, 40);
    playButton->setPos(690-80,196);
    connect(playButton,SIGNAL(clicked()),this,SLOT(connectingjoin()));
    scene->addItem(playButton);
}
//GUI
void GUI::servershowShipsMenu(){
    //server.write("Welcome to Marineford!!!");
    scene->clear();
    //int left_items_x_pos = this->width()/15;
    setBackground(":/Images/game_background_2.png");
    setMenuText("MARINEFORD", 45, this->width()/2, 30, true);
    setText("PLACE YOUR 10 SHIPS", 18, 200-40, 120, "white");

    Button* backButton = new Button(18, QString("Back"), Qt::red, Qt::darkRed, 95, 40);
    backButton->setPos(20,20);
    connect(backButton,SIGNAL(clicked()),this,SLOT(serverEnd()));
    //connect(backButton,SIGNAL(clicked()),this,SLOT(restartApplication()));

    scene->addItem(backButton);

    map = new Map();
    map->placeShips(500, 125, 10);

    int x=30, y=33;

    createShips(50+50+20+x, 160+y, 4, 7);
    createShips(50+50+20+x, 210+y, 3, 6);
    createShips(150+100+20+x, 210+y, 3, 5);
    createShips(50+50+20+x, 260+y, 2, 4);
    createShips(120+100+20+x, 260+y, 2, 3);
    createShips(190+150+20+x, 260+y, 2, 2);
    createShips(50+50+20+x, 310+y, 1, 1);
    createShips(90+100+20+x, 310+y, 1, 1);
    createShips(130+150+20+x, 310+y, 1, 1);
    createShips(170+200+20+x, 310+y, 1, 1);
}
//GUI
void GUI::clientshowShipsMenu(){
    //client.sendData("Welcome to Marineford!!!");
    scene->clear();
    setBackground(":/Images/game_background_2.png");
    setMenuText("MARINEFORD", 45, this->width()/2, 30, true);
    setText("PLACE YOUR 10 SHIPS", 18, 200-40, 120, "white");

    Button* backButton = new Button(18, QString("Back"), Qt::red, Qt::darkRed, 95, 40);
    backButton->setPos(20,20);

    connect(backButton,SIGNAL(clicked()),this,SLOT(restartApplication()));

    scene->addItem(backButton);

    map = new Map();
    map->placeShips(560, 125, 10);

    int x=30, y=33;
    playerShips.clear();

    createShips(50+50+20+x, 160+y, 4, 7);
    qDebug()<< "tamanho depois de 1"<< playerShips.size();
    createShips(50+50+20+x, 210+y, 3, 6);
    createShips(150+100+20+x, 210+y, 3, 5);
    createShips(50+50+20+x, 260+y, 2, 4);
    createShips(120+100+20+x, 260+y, 2, 3);
    createShips(190+150+20+x, 260+y, 2, 2);
    createShips(50+50+20+x, 310+y, 1, 1);
    createShips(90+100+20+x, 310+y, 1, 1);
    createShips(130+150+20+x, 310+y, 1, 1);
    createShips(170+200+20+x, 310+y, 1, 1);
}
//GUI
void GUI::showMenuSlot(){


    for(int i = 0; i<10; i++){
        for(int j= 0; j<10; j++){

            invalidPos[i][j]= 0;
        }
    }
    imPlaying= false;

    scene->clear();
    playerShips.clear();
    qDebug() << "SIZEE"<< playerShips.size();


    showMenu();
}

void GUI::connectingcreate()
{
    scene->clear();
    // Start the server
    setBackground(":/Images/game_background.png");
    setMenuText("MARINEFORD", 45, this->width()/2, 30, true);
    setMenuText("WAITING FOR PLAYER...", 30, 250, 220, false);

    Button* backButton = new Button(18, QString("Quit"), Qt::red, Qt::darkRed, 80, 40);
    backButton->setPos(20,20);
    connect(backButton,SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(backButton);

    if (server.listen(QHostAddress::Any, 12345)) {
        std::cout << "Server listening on port 12345..." << std::endl;
    } else {
        std::cout << "Failed to start server." << std::endl;
        return;  // Do not proceed if the server fails to start
    }
    std::cout << "Waiting for a connection..." << std::endl;
}
//GUI
void GUI::newConnection()
{
    //server.incomingConnection();
    imServer = true;
    imPlaying = true;
    servershowShipsMenu();
    std::cout << "Client connected!" << std::endl;
}

//GUI
void GUI::connectingjoin() {
    std::cout << "start join" << std::endl;

    client.connectToServer(ip, 12345);
}
//GUI
void GUI::onClientConnected() {
    //client.sendData("Hello from client!");
    clientshowShipsMenu();

}
Button::Button(int size, QString name, QColor color, QColor hover_color, int width, int height, QGraphicsItem *parent): QGraphicsPathItem(parent) {
    btt_color = color;
    btt_hover_color = hover_color;

    QPainterPath rect;
    //qreal width = 200;
    //qreal height = 50;
    qreal cornerRadius = 1;
    rect.addRoundedRect(0, 0, width, height, cornerRadius, cornerRadius);
    setPath(rect);

    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(btt_color);
    setBrush(brush);

    QPen pen;
    pen.setStyle(Qt::SolidLine);
    pen.setColor(btt_hover_color);
    pen.setWidth(1);
    setPen(pen);

    text = new QGraphicsTextItem(name,this);
    text->setDefaultTextColor("white");
    QFont btt_font("Courier", size, QFont::ExtraBold);
    text->setFont(btt_font);
    int xPos = width/2 - text->boundingRect().width()/2;
    int yPos = height/2 - text->boundingRect().height()/2;
    text->setPos(xPos,yPos);

    setAcceptHoverEvents(true);
}
void Button::mousePressEvent(QGraphicsSceneMouseEvent *event){
    emit clicked();
}
void Button::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(btt_hover_color);
    setBrush(brush);
    QPointF oldCenter = boundingRect().center();
    qreal scaleFactor = 0.96;
    setTransformOriginPoint(oldCenter);
    setScale(scaleFactor);

    setCursor(Qt::PointingHandCursor);
}
void Button::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(btt_color);
    setBrush(brush);

    QPointF oldCenter = boundingRect().center();
    qreal scaleFactor = 1;
    setTransformOriginPoint(oldCenter);
    setScale(scaleFactor);

    setCursor(Qt::ArrowCursor);

}
