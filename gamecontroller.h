#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include "../engine.h"
#include "stick.h"

class GameController : public QObject
{
    Q_OBJECT
    typedef engine::move_t mv;
public:
    explicit GameController(QObject *, QGraphicsScene*);

private:
    engine *pal;
    int nrPiles;
    short stones[10];
    Stick* sticks[10][10];
    QGraphicsScene *scene;
    void deleteFromPile(int, int);
    void initializeGame();


public slots:
    void make_move(int, int);
    void stick_selected(int, int, bool);
    void reset_game();
};

#endif // GAMECONTROLLER_H
