#include "fitnessview.h"

/* Histogram */
Histogram::Histogram(qreal x, qreal y, qreal width, qreal height, QWidget *parent)
    :QGraphicsScene(x, y, width, height, (QObject*)parent)
{
    size = DEFAULT_HISTOGRAM_SIZE;
    data = NULL;
}

void Histogram::setSize(int nsize)
{
    nsize = size;
    /* redivide data in size */
}

void Histogram::setData(Generation *gen)
{
    data = gen;
}

void Histogram::draw()
{
    /* divide data in buckets */
    /* plot on canvas */
    return;
}

void Histogram::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QList<int> ids = ((HistogramBar*)mouseGrabberItem())->getIds();
    emit changedSelection(ids);
}

/* Histogram bar class */
HistogramBar::HistogramBar(qreal x, qreal y, qreal width, qreal height, QList<int> nids)
    :QGraphicsRectItem(x, y, width, height)
{
    ids = nids;
}

QList<int> HistogramBar::getIds()
{
    return ids;
}
