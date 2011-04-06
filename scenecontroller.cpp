#include "scenecontroller.h"

SceneController::SceneController(QObject *parent, QVector<int> nrInPile) :
    QGraphicsScene(parent), sceneWidth(251), sceneHeight(231)
{
    setSceneRect(0, 0, sceneWidth, sceneHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);

    initializeGame(nrInPile);
}

void SceneController::initializeGame(QVector<int> nrInPile)
{
    int pm = 4;
    int allPileHeight = (20 + pm) * nrInPile.size() + pm;

    for (int p = 0; p != nrInPile.size(); ++p) {
        for (int s = 0; s != nrInPile[p]; ++s) {
            Stick *stk = new Stick(0, p, s);
            sticks[p].append(stk);

            stk->setPos(s * 10, (sceneHeight - allPileHeight) / 2 + pm + (pm + 20) * p);
            stk->setColor(QColor(rand() % 255, rand() % 255, rand() % 255 ));

            connect(stk,  SIGNAL( hovered(int, int, bool) ),
                    this, SLOT  ( stick_selected(int, int, bool) ) );

            connect(stk,    SIGNAL( clicked(int, int) ),
                    this,   SLOT  ( stick_clicked(int, int) ) );

            addItem(stk);
        }
    }
}

void SceneController::stick_selected(int pile, int nr, bool selected)
{
    double op;
    if (selected) op = .2;
    else op = 1;

    for (int i = nr; i != sticks[pile].size(); ++i)
        sticks[pile][i]->setOpacity(op);
}

void SceneController::deleteFromPile(int pile, int nr)
{
    int orig = sticks[pile].size() - nr;
    for ( int i = sticks[pile].size() - 1; i >= orig; --i ) {
        delete sticks[pile][i];
        sticks[pile].pop_back();
    }
}

void SceneController::stick_clicked(int pile, int stickNr)
{
    int aux = sticks[pile].size() - stickNr;
    deleteFromPile(pile, aux);
    emit make_move(pile, aux);
}

void SceneController::resetGame(QVector<int> nrInPile)
{
    for (int i = 0; i != 10; ++i)
        deleteFromPile(i, sticks[i].size());
    initializeGame(nrInPile);
}