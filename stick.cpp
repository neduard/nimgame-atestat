#include "stick.h"

#include <QGraphicsObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOption>
#include <stdio.h>
#include <QDebug>
#include <QObject>

Stick::Stick(QGraphicsItem *parent, int pile, int stone) : QGraphicsObject(parent)
{
    width = 5; height = 20;
    setAcceptsHoverEvents(true);
    pileNr = pile;
    stoneNr = stone;
}

QRectF Stick::boundingRect() const
{
    qreal adjust = 1;
    return QRectF(-2 - adjust, -25 - adjust,
                  width + adjust, height + adjust);
}

void Stick::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(color);
    painter->drawRect(-2,-25,width,height);
}

void Stick::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked(pileNr, stoneNr);
}

void Stick::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    emit hovered(pileNr, stoneNr, true);
}

void Stick::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    emit hovered(pileNr, stoneNr, false);
}

void Stick::setColor(QColor c)
{
    color = c;
}
