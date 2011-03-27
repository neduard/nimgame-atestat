#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "../engine.h"
#include "stick.h"
#include "scenecontroller.h"
#include "ui_mainwindow.h"

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
    engine *pal;
    SceneController *scene;

public slots:
    void make_move(int, int);
    void reset_game();
};

#endif // GAMECONTROLLER_H
