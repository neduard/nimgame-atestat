#ifndef STICK_H
#define STICK_H

#include <QGraphicsItem>
#include <QGraphicsObject>
#include <QColor>

class Stick : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit Stick(QGraphicsItem*, int, int);
    void paint ( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget );
    QRectF boundingRect() const;
    void setColor(QColor);

signals:
    void hovered(int, int, bool);
    void clicked(int, int);

private:
    QColor color;
    int pileNr, stoneNr;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
};

#endif // STICK_H
