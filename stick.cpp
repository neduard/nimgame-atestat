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
    setAcceptsHoverEvents(true);
    pileNr = pile;
    stoneNr = stone;
}

QRectF Stick::boundingRect() const
{
    qreal adjust = 1;
    return QRectF(-2 - adjust, -25 - adjust,
                  4 + adjust, 50 + adjust);
}

void Stick::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    option; widget;
    painter->setBrush(color);
    painter->drawRect(-2,-25,4,50);
}

void Stick::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug("You fu**ing clicked meh! I'm in pile nr. %d, stick nr. %d", pileNr, stoneNr);
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
