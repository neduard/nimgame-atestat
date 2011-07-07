#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QDialog>
#include "ui_newGameDialog.h"

class newGameValues
{
public:
    int nrRows;
    double mistakeChance;
    bool normal;
};

class newGameDialog : public QDialog
{
    Q_OBJECT
    Ui::ngDialog *ui;
    newGameValues ngv;
public:
    explicit newGameDialog(QWidget *parent, newGameValues);
    ~newGameDialog();
    newGameValues getNewGameValues();

signals:

private slots:
    void copyValues();

};

#endif // NEWGAMEDIALOG_H
