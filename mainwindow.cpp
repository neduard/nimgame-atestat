#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "stick.h"
#include "gamecontroller.h"

#include <stdio.h>
#include <QGraphicsScene>
#include <QtGui>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    gc = new GameController(this, this, ui = new Ui::MainWindow);
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

