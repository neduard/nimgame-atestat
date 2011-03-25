#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <QObject>
#include <QGraphicsScene>
#include <QAbstractButton>
#include <QSpinBox>
#include "../engine.h"
#include "stick.h"

class GameController : public QObject
{
    Q_OBJECT
    typedef engine::move_t mv;
public:
    GameController(QObject *, QGraphicsScene*, QAbstractButton*, QSpinBox*);

private:
    engine *pal;
    int nrPiles;
    short stones[10];
    Stick* sticks[10][50];
    QGraphicsScene *scene;
    void deleteFromPile(int, int);
    void initializeGame();
    QAbstractButton *winBtn;
    QSpinBox *spbox;



public slots:
    void make_move(int, int);
    void stick_selected(int, int, bool);
    void reset_game();
};

#endif // GAMECONTROLLER_H
