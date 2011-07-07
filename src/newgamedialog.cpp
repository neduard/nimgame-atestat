#include "newgamedialog.h"

newGameDialog::newGameDialog(QWidget *parent, newGameValues x) :
    QDialog(parent)
{
    ui = new Ui::ngDialog;
    ui->setupUi(this);
    connect(this, SIGNAL(accepted()), this, SLOT(copyValues()));
    ngv = x;
    ui->diffSpnBx->setValue((int)((double)ui->diffSpnBx->maximum() * (1.0 - ngv.mistakeChance)));
    if (ngv.normal) ui->rdBtnNormal->setChecked(true);
    else ui->rdBtnMisere->setChecked(true);
    ui->rowsSpnBx->setValue(ngv.nrRows);
}
newGameDialog::~newGameDialog()
{
    delete ui;
}

newGameValues newGameDialog::getNewGameValues()
{
    return ngv;
}

void newGameDialog::copyValues()
{
    ngv.mistakeChance = ((double)ui->diffSpnBx->maximum() - (double)ui->diffSpnBx->value()) / (double)ui->diffSpnBx->maximum();
    ngv.normal = ui->rdBtnNormal->isChecked();
    ngv.nrRows = ui->rowsSpnBx->value();
}
