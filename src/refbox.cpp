#include "refbox.h"

Refbox::Refbox(QList<QColor> box_lines, QStringList box_labels, QPointF box_pos)
{
    lines = box_lines;
    labels = box_labels;
    pos = box_pos;
    elements = NULL;
}

void Refbox::setPos(QPointF npos)
{
    pos = npos;
}

QPointF Refbox::getPos()
{
    if(elements != NULL)
        return elements->scenePos();
    else
        return pos;
}

void Refbox::draw(QGraphicsScene *canvas)
{
    QPointF pointPos = pos;

    elements = new QGraphicsItemGroup();
    QGraphicsRectItem *bound = new QGraphicsRectItem(pointPos.x(), pointPos.y(), 2, 2);
    bound->setBrush(QBrush(Style::refboxBGColor));
    bound->setPen(Style::refboxBorder);
    elements->addToGroup(bound);

    pointPos += QPointF(Style::refboxPadding / 2, Style::refboxPadding);

    float maxTextLen = FLT_MIN;
    float boxHeight = 0;
    for(int i = 0; i < lines.length(); i++)
    {
        QGraphicsTextItem *text = new QGraphicsTextItem(labels[i]);
        QRectF bbox = text->boundingRect();
        QGraphicsLineItem *line = new QGraphicsLineItem(pointPos.x(),
                                                        pointPos.y() + bbox.height()/2,
                                                        pointPos.x() + Style::refboxLineLen,
                                                        pointPos.y() + bbox.height()/2);
        line->setPen(QPen(lines[i], Style::edgeWeight));
        text->setPos(QPointF(pointPos.x() + Style::refboxLineLen + 5, pointPos.y()));
        if(bbox.width() > maxTextLen)
            maxTextLen = bbox.width();

        elements->addToGroup(text);
        elements->addToGroup(line);

        pointPos += QPointF(0, bbox.height() * 1.5);
        boxHeight += bbox.height() * 1.5;
    }

    bound->setRect(pos.x(), pos.y(), 
                   maxTextLen + Style::refboxLineLen + Style::refboxPadding * 3,
                   boxHeight + Style::refboxPadding);
    elements->setFlags(QGraphicsItem::ItemIsMovable);
    canvas->addItem(elements);
}
