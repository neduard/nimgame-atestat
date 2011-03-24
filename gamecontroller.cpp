#include "gamecontroller.h"
#include "stick.h"
#include "../engine.h"

#include <QDebug>


GameController::GameController(QObject *parent, QGraphicsScene *s) :
    scene(s), QObject(parent), nrPiles(3), pal(nrPiles, 0, 0, 0)
{
    stones[1] = 3; stones[2] = 5; stones[3] = 7;
    Stick* aux;
    for (int pile = 1; pile <= 3; ++pile)
        for (int stick = 1; stick <= stones[pile]; ++stick) {
            aux = sticks[pile][stick] = new Stick(0, pile, stick);
            aux->setPos((stick - 1) * 20, (pile - 1) * 80);
            aux->setColor(QColor(rand() % 255, rand() % 255, rand() % 255 ));
            scene->addItem(aux);
            connect(aux, SIGNAL(hovered(int,int)), this, SLOT(stick_selected(int,int)));
        }
}

void GameController::make_move()
{
    qDebug("making a move");
}

void GameController::stick_selected(int nr, int pile)
{
    qDebug("stick %d from pile %d has been selected", nr, pile);

}
