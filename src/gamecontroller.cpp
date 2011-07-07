#include "gamecontroller.h"


#include <QDebug>
#include <QMessageBox>
#include <QDialog>
#include <vector>
#include <assert.h>


GameController::GameController(QObject *p, QMainWindow* w, Ui::MainWindow* u) :
    QObject(p), ui(u), pal(0)
{
    ui->setupUi(mainWindow = w);
    scene = new SceneController(this);

    ui->gameView->setScene(scene);
    ui->gameView->setCacheMode(QGraphicsView::CacheBackground);
    ui->gameView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->gameView->setDragMode(QGraphicsView::NoDrag);

    connect(ui->actionNew,   SIGNAL(triggered()),    this, SLOT(new_game()));
    connect(ui->actionHint,  SIGNAL(triggered()),    this, SLOT(show_optimum_move()));
    connect(ui->actionAbout, SIGNAL(triggered()),    this, SLOT(about_game()));
    connect(ui->actionForce, SIGNAL(triggered()),    this, SLOT(force_move()));

    connect(scene,       SIGNAL(make_move(int,int)), this, SLOT(make_move(int,int)));

    ngv.mistakeChance = 0.0;
    ngv.normal = false;
    ngv.nrRows = 3;

    new_game();
    ui->gameView->show();
}

void GameController::make_move(int pile, int nr)
{

    mv pm = pal->move(engine::move_t(pile, nr));
    assert(pm.nrTaken || pal->is_ended());
    scene->deleteFromPile(pm.pile, pm.nrTaken);

    winningHandler();

    if (pal->is_ended()) {
        QMessageBox mb;
        if (pal->win()) mb.setText("The Game! You lost it!");
        else mb.setText("Congrats! You won the game!");
        mb.exec();
        new_game(false);
    }
}

void GameController::new_game(bool showDialog )
{
    if (showDialog) {
        newGameDialog ngd(mainWindow, ngv);
        if (!ngd.exec()) {
            if (!pal) exit(0);
            return;
        }
        ngv = ngd.getNewGameValues();
    }

    if (pal) { delete pal; pal = 0; }

    std::vector<short> aux;
    for (short i = 1; i <= ngv.nrRows; ++i)
        aux.push_back(i * 2 + 1);

    pal = new engine(aux, ngv.mistakeChance, ngv.normal);
    scene->initializeGame(aux);
    winningHandler();
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

void GameController::winningHandler()
{
    ui->winning->setChecked(!pal->win());
    if (!pal->win()) {
        ui->statusbar->showMessage("you can win!");
    }
    else ui->statusbar->showMessage("you'll lose");
}

void GameController::force_move()
{
    make_move(0, 0);
}
