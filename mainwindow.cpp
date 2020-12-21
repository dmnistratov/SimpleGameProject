#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gamefield.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ended = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start()
{
    if (!ended) return;
    ended = false;
    scene = new QGraphicsScene(0,0, 800, 800, this);
    ui->gamefiledView->setScene(scene);
    player = Player::GetInstance();
    player->setHP(2);
    enemyF = new Enemy<Fire>();
    enemyI = new Enemy<Ice>();
    enemyE = new Enemy<Earth>();
    gamefield = GameField::GetInstance();
    timer = new QTimer(this);
    size_t m = 10;
    size_t n = 10;
    gamefield->setSize(n,m);
    gamefield->createField();
    GameFieldIterator st_test = gamefield->begin();
    GameFieldIterator end = gamefield->end();
    gamefield->generateMap();
    for (; st_test != end; ++st_test){
        scene->addItem(*st_test);
        if ((*st_test)->getType() == Cell::entry){
            player->setX((*st_test)->getX());
            player->setY((*st_test)->getY());
            player->setScale(MAP_RESOLUTION/m);
        }
        else if ((*st_test)->getType() == Cell::exit){
            enemyF->setX((*st_test)->getX());
            enemyF->setY((*st_test)->getY());
            enemyF->setScale(MAP_RESOLUTION/m);
        }
        if ((*st_test)->getType() == Cell::filled){
            enemyI->setX((*st_test)->getX());
            enemyI->setY((*st_test)->getY());
            enemyI->setScale(MAP_RESOLUTION/m);
            //
            enemyE->setX(m-2);
            enemyE->setY(3);
            enemyE->setScale(MAP_RESOLUTION/m);
        }
    }
    scene->addItem(gamefield->keyb);
    scene->addItem(gamefield->tpb);
    scene->addItem(gamefield->rainb);
    scene->addItem(player);
    scene->addItem(enemyI);
    scene->addItem(enemyE);
    scene->addItem(enemyF);
    player->setType(Player::ice);
    connect(timer, SIGNAL(timeout()), this, SLOT(updater()));
    timer->start(60);
    this->setFocus();
}

void MainWindow::end()
{
    if (ended) return;
    player->setKeys(0);
    scene->removeItem(player);
    scene->removeItem(enemyI);
    scene->removeItem(enemyF);
    scene->removeItem(enemyE);
    scene->removeItem(gamefield->keyb);
    scene->removeItem(gamefield->tpb);
    scene->removeItem(gamefield->rainb);
    GameFieldIterator st_test = gamefield->begin();
    GameFieldIterator end = gamefield->end();
    for (; st_test != end; ++st_test)
        scene->removeItem(*st_test);
 //   gamefield->clearField(); #WARNING test for memleak
    scene->clear();
    scene->update();
    delete timer;
    ended = true;
}

void MainWindow::playerMove()
{
    //TODO divide next cell position by frames to animate character (1/frames of sprite animation)
    switch (player->getDirection()) {
    case Player::left:
        if(!gamefield->isCollide(player->getX()-1, player->getY())){
            player->setX(player->getX()-1);
        }
        player->setDirection(Player::idle);
        break;
    case Player::right:
        if(!gamefield->isCollide(player->getX()+1, player->getY())){
            player->setX(player->getX()+1);
        }
        player->setDirection(Player::idle);
        break;
    case Player::up:
        if(!gamefield->isCollide(player->getX(), player->getY()-1)){
            player->setY(player->getY()-1);
        }
        player->setDirection(Player::idle);
        break;
    case Player::down:
        if(!gamefield->isCollide(player->getX(), player->getY()+1)){
            player->setY(player->getY()+1);
        }
        player->setDirection(Player::idle);
        break;
    case Player::idle:
        break;
    }

    if (gamefield->isExit(player->getX(), player->getY()) && (player->getKeys() == 1)){
        timer->stop();
        end();
        QMessageBox::information(0, QString("Information"), QString("Game Ended"), QMessageBox::Ok);
    }
}

template<typename L>
void MainWindow::enemyMove(L enemy)
{
    if (player->getDirection() != Player::idle)
    {
        int dir = 1+rand()%4;
        switch (dir) {
        case 1:
            if(!gamefield->isCollide(enemy->getX()+1, enemy->getY()))
                enemy->setX(enemy->getX()+1);
            break;
        case 2:
            if(!gamefield->isCollide(enemy->getX()-1, enemy->getY()))
                enemy->setX(enemy->getX()-1);
            break;
        case 3:
            if(!gamefield->isCollide(enemy->getX(), enemy->getY()+1))
                enemy->setY(enemy->getY()+1);
            break;
        case 4:
            if(!gamefield->isCollide(enemy->getX(), enemy->getY()-1))
                enemy->setY(enemy->getY()-1);
            break;
        }
    }

    if (player->getX() == enemy->getX() && player->getY() == enemy->getY())
    {
        /*timer->stop();
        end();
        QMessageBox::information(0, QString("Information"), QString("Game Ended\nYou Died!"), QMessageBox::Ok);*/
        *enemy + player;
        *player + enemy;
        std::cout << player->getHP() << std::endl;
    }
}

void MainWindow::playerPick()
{
    IBonus* temp = nullptr;
    if ( (temp = gamefield->pickup(player->getX(), player->getY())) != nullptr)
        *player + temp;
}

void MainWindow::updater()
{
    if (player->isDead())
    {
        timer->stop();
        end();
        QMessageBox::information(0, QString("Information"), QString("Game Ended\nYou Died!"), QMessageBox::Ok);
    }
    enemyMove(enemyE);
    enemyMove(enemyF);
    enemyMove(enemyI);
    playerMove();

    this->setFocus();
    scene->update();
}

void MainWindow::on_start_clicked()
{
    start();
}

void MainWindow::on_save_clicked()
{
    if (ended) return ;
    QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save File as"), "",
            tr("Save File (*.txt)"));
    try {
        saveload = new SaveLoad(fileName.toStdString());
    }  catch (std::runtime_error &e) {
        QMessageBox::warning(0, QString(" "), QString(e.what()), QMessageBox::Ok);
        return;
    }
    saveload->save(gamefield->saveMap());
    saveload->save(enemyF->saveEnemy());
    saveload->save(enemyI->saveEnemy());
    saveload->save(enemyE->saveEnemy());
    saveload->save(player->savePlayer());
}

void MainWindow::on_load_clicked()
{
    std::cout << "load" << std::endl;
    QString fileName = QFileDialog::getOpenFileName(this,
           tr("Open Load File"), "",
           tr("Load File (*.txt)"));
    try {
        saveload = new SaveLoad(fileName.toStdString());
    }  catch (std::runtime_error &e) {
        QMessageBox::warning(0, QString(" "), QString(e.what()), QMessageBox::Ok);
        return;
    }
    if (ended)
    {
        start();
    }
    std::ofstream tempsv("temp.txt");
    std::string pull;
    SaveLoad* tempsl = new SaveLoad("temp.txt");
    tempsl->save(gamefield->saveMap());
    tempsl->save(enemyF->saveEnemy());
    tempsl->save(enemyI->saveEnemy());
    tempsl->save(enemyE->saveEnemy());
    tempsl->save(player->savePlayer());
    try {
        gamefield->createMap(pull = saveload->loadline());
        gamefield->loadKey(pull = saveload->loadline());
        gamefield->loadTp(pull = saveload->loadline());
        gamefield->loadRainb(pull = saveload->loadline());
        enemyF->loadEnemy(pull = saveload->loadline());
        enemyI->loadEnemy(pull = saveload->loadline());
        enemyE->loadEnemy(pull = saveload->loadline());
        player->loadPlayer(pull = saveload->loadline());
    }  catch (...) {
        QMessageBox::warning(0, QString(" "), QString("File load error!"), QMessageBox::Ok);
        try {
            saveload = new SaveLoad("temp.txt");
        }  catch (std::runtime_error &e) {
            QMessageBox::warning(0, QString(" "), QString(e.what()), QMessageBox::Ok);
            return;
        }
    }
    std::cout << saveload->loadline() << std::endl;
}


void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (ended) return;
    switch (event->key()) {
    case Qt::Key_W :
        std::cout << "W" << std::endl;
        player->setDirection(Player::up);
        break;
    case Qt::Key_A :
        std::cout << "A" << std::endl;
        player->setDirection(Player::left);
        break;
    case Qt::Key_S:
        std::cout << "S" << std::endl;
        player->setDirection(Player::down);
        break;
    case Qt::Key_D:
        std::cout << "D" << std::endl;
        player->setDirection(Player::right);
        break;
    case Qt::Key_E:
        std::cout << "E" << std::endl;
        playerPick();
        break;
    }
    return;
}

