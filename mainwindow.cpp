#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stick.h"
#include "gamecontroller.h"

#include <stdio.h>
#include <QGraphicsScene>
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);


    scene = new QGraphicsScene;

    scene->setSceneRect(0, 0, 100, 150);
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);

    gc = new GameController(this, scene);

    connect(ui->rstBtn, SIGNAL(clicked(bool)), gc, SLOT(reset_game()));

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

