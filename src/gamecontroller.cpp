#include "gamecontroller.h"

#include <QDebug>
#include <QMessageBox>
#include <assert.h>


GameController::GameController(QObject *p, QMainWindow* w, Ui::MainWindow* u) :
    QObject(p), ui(u)
{
    QVector<int> initStones;
    short initStones2[] = {3, 5, 7};
    initStones << 3 << 5 << 7;
    ui->setupUi(w);


    scene = new SceneController(this, initStones);
    pal = new engine(3, initStones2, (5.0 - ui->spinBox_2->value()) / 5.0);



    ui->winning->setChecked(!pal->win());

    ui->gameView->setScene(scene);
    ui->gameView->setCacheMode(QGraphicsView::CacheBackground);
    ui->gameView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->gameView->setDragMode(QGraphicsView::NoDrag);


    connect(ui->anlzBtn, SIGNAL(clicked()),          this, SLOT(show_optimum_move()));
    connect(ui->rstBtn,  SIGNAL(clicked(bool)),      this, SLOT(reset_game()));
    connect(scene,       SIGNAL(make_move(int,int)), this, SLOT(make_move(int,int)));
    connect(ui->aboutBtn, SIGNAL(clicked()),          this, SLOT(about_game()));

    ui->gameView->show();
}

void GameController::make_move(int pile, int nr)
{

    mv pm = pal->move(pile, nr);
    assert(pm.nrTaken || pal->is_ended());
    scene->deleteFromPile(pm.pile, pm.nrTaken);
    ui->winning->setChecked(!pal->win());

    if (pal->is_ended()) {
        QMessageBox mb;
        if (pal->win()) mb.setText("The Game! You lost it!");
        else mb.setText("Congrats! You won the game!");
        mb.exec();
        reset_game();
    }
}

void GameController::reset_game()
{
    QVector<int> aux;
    for (int i = 1; i <= ui->spinBox->value(); ++i)
        aux.append(i * 2 + 1);
    delete pal;
    pal = new engine(ui->spinBox->value(), 0, (5.0 - ui->spinBox_2->value()) / 5.0);
    scene->resetGame(aux);
    ui->winning->setChecked(!pal->win());
}

void GameController::show_optimum_move()
{
    QMessageBox mb;
    if (!pal->win()) {
        char buff[50];
        engine::move_t m = pal->detOptimumMove();
        sprintf(buff, "Take %d sticks from pile number %d. \n",  m.nrTaken, m.pile + 1);
        mb.setText(buff);
    }
    else mb.setText("Sorry, you can't win.");
    mb.exec();
}

void GameController::about_game()
{
   QMessageBox mb;

   mb.setText("Made by: \n    Eduard Nicodei  \n    Marius Dumitrescu    \n \n Vers 1.0 \n");

   mb.exec();


}
