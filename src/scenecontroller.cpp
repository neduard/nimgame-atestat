#include "scenecontroller.h"

SceneController::SceneController(QObject *parent) :
    QGraphicsScene(parent), sceneWidth(251), sceneHeight(231)
{
    setSceneRect(0, 0, sceneWidth, sceneHeight);
    setItemIndexMethod(QGraphicsScene::NoIndex);
}

void SceneController::initializeGame(std::vector<short> nrInPile)
{
    int pm = 4;
    int allPileHeight = (20 + pm) * nrInPile.size() + pm;
    int nrCurPile = 3,cntCurPile = 3;

    // we might have sticks left over from previous game (this is from the old reset_game() function)
    for (int i = 0; i != 10; ++i)
        if (sticks[i].size())
            deleteFromPile(i, sticks[i].size());

    for (unsigned p = 0; p != nrInPile.size(); ++p) {
        QColor rowColor(rand() % 256, rand() % 256, rand() % 256);
        for (int s = 0; s != nrInPile[p]; ++s) {
            Stick *stk = new Stick(0, p, s);
            sticks[p].append(stk);
            if ( !cntCurPile ) {
                nrCurPile += 2;
                cntCurPile = nrCurPile;
            }
            stk->setPos(sceneWidth/2 - nrCurPile*5 + s * 10 , (sceneHeight - allPileHeight) / 2 + pm + (pm + 20) * p);
            stk->setColor(rowColor);
            cntCurPile--;
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
    assert(nr <= sticks[pile].size());
    for ( int i = 0; i != nr; ++i ) {
        delete sticks[pile].last();
        sticks[pile].pop_back();
    }
}

void SceneController::stick_clicked(int pile, int stickNr)
{
    int aux = sticks[pile].size() - stickNr;
    deleteFromPile(pile, aux);
    emit make_move(pile, aux);
}
