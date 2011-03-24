#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include "../engine.h"
#include "stick.h"

class GameController : public QObject
{
    Q_OBJECT
public:
    explicit GameController(QObject *, QGraphicsScene*);

private:
    engine pal;
    int nrPiles;
    short stones[10];
    Stick* sticks[10][10];
    QGraphicsScene *scene;


public slots:
    void make_move();
    void stick_selected(int, int);
};

#endif // GAMECONTROLLER_H
