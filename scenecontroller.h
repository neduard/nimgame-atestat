#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include "stick.h"
#include <QGraphicsScene>


class SceneController : public QGraphicsScene
{
    Q_OBJECT
    QVector<Stick*> sticks[10];
    const int sceneWidth, sceneHeight;

    void initializeGame(QVector<int>);


public:
    explicit SceneController(QObject*, QVector<int>);
    void deleteFromPile(int, int);
    void resetGame(QVector<int>);

signals:
    void make_move(int, int);

public slots:
    void stick_selected(int, int, bool);
    void stick_clicked(int, int);
};

#endif // SCENECONTROLLER_H
