#include <QGraphicsItemGroup>
#include <QGraphicsLineItem>
#include <QGraphicsTextItem>
#include <QGraphicsScene>
#include <QList>
#include <QStringList>
#include <QPen>

#include <cfloat>

#include "style.h"

class Refbox
{
    //TODO: implement destructor
    public:
        Refbox(QList<QColor> box_lines, QStringList box_labels, QPointF pos);
        void setPos(QPointF npos);
        QPointF getPos();
        void draw(QGraphicsScene *canvas);

    private:
        QPointF pos;
        QList<QColor> lines;
        QStringList labels;
        QGraphicsItemGroup *elements;
};
