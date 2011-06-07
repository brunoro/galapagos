#include "refbox.h"

Refbox::Refbox(QList<QColor> box_lines, QStringList box_labels)
{
    lines = box_lines;
    labels = box_labels;
    pos = QPointF(0, 0);
    elements = new QGraphicsItemGroup();
}

void Refbox::setPos(QPointF npos)
{
    QPointF diff = npos - pos;
    pos = npos;
    elements->setPos(elements->pos() + diff);
}

void Refbox::draw(QGraphicsScene *canvas)
{
    QPointF pointPos = pos;

    QGraphicsRectItem *bound = new QGraphicsRectItem(pointPos.x(), pointPos.y(), 2, 2);
    bound->setBrush(QBrush(Style::refboxBGColor));
    bound->setPen(Style::refboxBorder);
    elements->addToGroup(bound);

    pointPos += QPointF(Style::refboxPadding / 2, Style::refboxPadding);

    float maxTextLen = FLT_MIN;
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

        if(i != lines.length() - 1)
            pointPos += QPointF(0, bbox.height() * 1.5);
        else
            pointPos += QPointF(0, bbox.height());
    }

    bound->setRect(pos.x(), pos.y(), 
                   pointPos.x() + maxTextLen + Style::refboxLineLen + Style::refboxPadding * 3,
                   pointPos.y() + Style::refboxPadding);
    elements->setFlags(QGraphicsItem::ItemIsMovable);
    canvas->addItem(elements);
}
