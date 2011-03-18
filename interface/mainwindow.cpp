#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stick.h"

#include <stdio.h>
#include <QGraphicsScene>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    connect(ui->mkMoveBtn, SIGNAL(clicked(bool)), this, SLOT(make_a_move()));

    scene = new QGraphicsScene;

    scene->setSceneRect(-50, -50, 100, 100);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    /**
      HAIR WE ADDZ TEH STICKZORS
      THOU SHALT ADDEH MOAR FOR
      GREAT LULZ
      **/

    // on a more serious note, we shouldn't add them here,
    // but rather in a separate class or smthing
    Stick *st = new Stick(1,1);
    st->setPos(0,0);
    scene->addItem(st);

    QTimer *timer = new QTimer;
    QObject::connect(timer, SIGNAL(timeout()), scene, SLOT(advance() ));
    timer->start(1000 / 33);

    ui->gameView->setScene(scene);
    ui->gameView->setCacheMode(QGraphicsView::CacheBackground);
    ui->gameView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->gameView->setDragMode(QGraphicsView::NoDrag);
    //ui->gameView->resize(550, 550);
    ui->gameView->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void MainWindow::make_a_move()
{
    printf("make a move already....\n");
    fflush(stdout);
}
