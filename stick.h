#ifndef STICK_H
#define STICK_H

#include <QGraphicsItem>

class Stick : public QGraphicsItem
{
public:
    Stick(int, int);
    void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
    QRectF boundingRect() const;

private:

    int pileNr, stoneNr;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // STICK_H
