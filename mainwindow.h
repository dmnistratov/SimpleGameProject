#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QKeyEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "player.h"
#include "enemy.h"
#include "saveload.h"
#include "bonus.h"
#include "gamefield.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class State;
class StateContext;
class EnemyState;
class PlayerState;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void playerMove();
    template<typename L>
    void enemyMove(L enemy);
private:
    void start();
    void end();
    void playerPick();
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QTimer *timer;

    Player *player;
    Enemy<Fire> *enemyF;
    Enemy<Ice> *enemyI;
    Enemy<Earth> *enemyE;
    GameField* gamefield;
    SaveLoad* saveload;
    bool ended;

public slots:
    void updater();
    void on_start_clicked();
    void on_save_clicked();
    void on_load_clicked();

protected:
    void keyPressEvent(QKeyEvent *event);

};
#endif // MAINWINDOW_H
