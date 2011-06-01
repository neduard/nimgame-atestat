#include "gamecontroller.h"

#include <QDebug>
#include <QMessageBox>
#include <vector>
#include <assert.h>


GameController::GameController(QObject *p, QMainWindow* w, Ui::MainWindow* u) :
    QObject(p), ui(u), pal(0)
{
    ui->setupUi(w);
    scene = new SceneController(this);

    ui->gameView->setScene(scene);
    ui->gameView->setCacheMode(QGraphicsView::CacheBackground);
    ui->gameView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->gameView->setDragMode(QGraphicsView::NoDrag);


    connect(ui->anlzBtn, SIGNAL(clicked()),          this, SLOT(show_optimum_move()));
    connect(ui->rstBtn,  SIGNAL(clicked(bool)),      this, SLOT(new_game()));
    connect(scene,       SIGNAL(make_move(int,int)), this, SLOT(make_move(int,int)));
    connect(ui->aboutBtn, SIGNAL(clicked()),          this, SLOT(about_game()));

    new_game();
    ui->gameView->show();
}

void GameController::make_move(int pile, int nr)
{

    mv pm = pal->move(engine::move_t(pile, nr));
    assert(pm.nrTaken || pal->is_ended());
    scene->deleteFromPile(pm.pile, pm.nrTaken);
    ui->winning->setChecked(!pal->win());

    if (pal->is_ended()) {
        QMessageBox mb;
        if (pal->win()) mb.setText("The Game! You lost it!");
        else mb.setText("Congrats! You won the game!");
        mb.exec();
        new_game();
    }
}

void GameController::new_game()
{
    std::vector<short> aux;
    if (pal) delete pal;

    for (short i = 1; i <= ui->rowsSpnBx->value(); ++i)
        aux.push_back(i * 2 + 1);
    pal = new engine(aux, (5.0 - ui->diffSpnBx->value()) / 5.0);
    scene->initializeGame(aux);
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

   mb.setText("Back in the prototype phase.\nOwner eddzor");

   mb.exec();


}
