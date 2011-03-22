#include "stick.h"

#include <QPainter>
#include <QStyleOption>
#include <stdio.h>
#include <QDebug>

Stick::Stick(int pile, int nr)
{
    pileNr = pile;
    stoneNr = nr;
}

QRectF Stick::boundingRect() const
{
    qreal adjust = 1;
    return QRectF(-2 - adjust, -25 - adjust,
                  4 + adjust, 50 + adjust);
}

void Stick::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(Qt::cyan);
    painter->drawRect(-2,-25,4,50);
}

void Stick::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("You fu**ing clicked meh! I'm in pile nr. %d, stick nr. %d", pileNr, stoneNr);
}
