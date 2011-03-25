#include "gamecontroller.h"
#include "stick.h"
#include "../engine.h"

#include <QDebug>


GameController::GameController(QObject *parent, QGraphicsScene *s) :
    scene(s), QObject(parent), nrPiles(3), pal(0x0)
{
    initializeGame();
}

void GameController::make_move(int pile, int stickNr)
{

    mv pm = pal->move((mv){pile - 1, stones[pile] - stickNr + 1});
    deleteFromPile(pile, stones[pile] - stickNr + 1);
    deleteFromPile(pm.pile + 1, pm.nrTaken);
}

void GameController::deleteFromPile(int pile, int nr)
{
    for (int i = stones[pile] - nr + 1; i <= stones[pile]; ++i) {
        delete sticks[pile][i];
        sticks[pile][i] = 0;
    }
    stones[pile] -= nr;
}

void GameController::stick_selected(int pile, int nr, bool selected)
{
    double op;
    if (selected) {
        qDebug("stick %d from pile %d has been selected", nr, pile);
        op = .2;
    } else {
        qDebug("now it's deselected");
        op = 1;
    }

    for (int i = nr; i <= stones[pile]; ++i) {
        sticks[pile][i]->setOpacity(op);
    }
}

void GameController::reset_game()
{
    for (int p = 1; p <= nrPiles; ++p)
        for (int i = 1; i <= stones[p]; ++i) {
            delete sticks[p][i];
            sticks[p][i] = 0;
        }
    initializeGame();
}

void GameController::initializeGame()
{
    if (pal) {
        delete pal;
        pal = new engine(3, 0, 0);
    }
    else pal = new engine(3, 0 ,0);
    stones[1] = 3; stones[2] = 5; stones[3] = 7;
    Stick* aux;
    for (int pile = 1; pile <= 3; ++pile)
        for (int stick = 1; stick <= stones[pile]; ++stick) {
            aux = sticks[pile][stick] = new Stick(0, pile, stick);
            aux->setPos((stick - 1) * 20, (pile - 1) * 80);
            aux->setColor(QColor(rand() % 255, rand() % 255, rand() % 255 ));
            scene->addItem(aux);
            connect(aux,  SIGNAL( hovered(int, int, bool) ),
                    this, SLOT  ( stick_selected(int, int, bool) ) );
            connect(aux,  SIGNAL(clicked(int, int)),
                    this, SLOT(make_move(int, int)));
        }
}
