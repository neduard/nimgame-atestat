#ifndef SCENECONTROLLER_H
#define SCENECONTROLLER_H

#include "stick.h"
#include <assert.h>
#include <vector>
#include <QGraphicsScene>


class SceneController : public QGraphicsScene
{
    Q_OBJECT
    QVector<Stick*> sticks[10];
    const int sceneWidth, sceneHeight;

public:
    explicit SceneController(QObject*);
    void deleteFromPile(int, int);
    void initializeGame(std::vector<short>);

signals:
    void make_move(int, int);

public slots:
    void stick_selected(int, int, bool);
    void stick_clicked(int, int);
};

#endif // SCENECONTROLLER_H
