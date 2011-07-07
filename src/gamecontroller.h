#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "engine.h"
#include "stick.h"
#include "scenecontroller.h"
#include "ui_mainwindow.h"
#include "newgamedialog.h"

#include <QObject>
#include <QGraphicsScene>
#include <QAbstractButton>
#include <QSpinBox>
#include <QMainWindow>

class GameController : public QObject
{
    Q_OBJECT
    typedef engine::move_t mv;
public:
    GameController(QObject*, QMainWindow*, Ui::MainWindow*);

private:
    Ui::MainWindow *ui;
    QMainWindow *mainWindow;
    engine *pal;
    SceneController *scene;
    newGameValues ngv;
    void winningHandler();

public slots:
    void make_move(int, int);
    void force_move();
    void new_game(bool = true);
    void show_optimum_move();
    void about_game();
};

#endif // GAMECONTROLLER_H
