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
}

void Histogram::setData(Generation *gen)
{
    data = gen;
}

bool fitnessLessThan(const idfit &a, const idfit &b)
{
    return a.fit < b.fit;
}

void Histogram::draw()
{
    clear();
    /* normalize data */
    QList<struct idfit> pairs;
    for(int i = 0; i < data->population.length(); i++)
    {
        struct idfit pair_i;
        pair_i.id = i;
        pair_i.fit = data->population[i].fit;
        pairs.append(pair_i);
    }

    qSort(pairs.begin(), pairs.end(), fitnessLessThan);
    
    /* normalize and divide data 
     * normalizing = (fit - smallest) / biggest */
    // TODO: get full fitness space to normalize
    float smallest = pairs.first().fit,
          bigger = pairs.last().fit;
    
    float bucketSize = 1.0 / size;
    QVector< QList<int> > buckets(size);

    int bucketCount = 0;
    for(int i = 0; i < pairs.length(); i++)
    {
        pairs[i].fit = ((pairs[i].fit - smallest) / bigger);
        if(pairs[i].fit > bucketCount * bucketSize)
            bucketCount++;
        buckets[bucketCount].append(pairs[i].id);
    }

    /* draw bars */
    for(int i = 0; i < size; i++)
    {
        HistogramBar *bar_i = new HistogramBar(Style::histogramPadding,
                                               (Style::histogramHeight / size) * (size - i - 1),
                                               Style::histogramWidth * buckets[i].length() / pairs.length(),
                                               Style::histogramHeight / size, 
                                               buckets[i]);
        bar_i->setBrush(Style::histogramBarBrush);
        bar_i->setPen(Style::histogramBarPen);
        if(buckets[i].length() <= 1)
            bar_i->setToolTip(QString::number(buckets[i].length()) + " individual");
        else
            bar_i->setToolTip(QString::number(buckets[i].length()) + " individuals");
        addItem(bar_i);
    }

    /* draw axis */
    QGraphicsLineItem *xaxis = new QGraphicsLineItem(Style::histogramPadding, Style::histogramHeight,
                                                     Style::histogramWidth, Style::histogramHeight);
    QGraphicsLineItem *yaxis = new QGraphicsLineItem(Style::histogramPadding, Style::histogramPadding,
                                                     Style::histogramPadding, Style::histogramHeight);
    xaxis->setPen(Style::histogramAxisColor);
    yaxis->setPen(Style::histogramAxisColor);
    addItem(xaxis);
    addItem(yaxis);
}

void Histogram::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsItem *item = mouseGrabberItem();
    qDebug() << "clicked!" << item;
    if(item != NULL)
    {
        QList<int> ids = ((HistogramBar*)item)->getIds();
        emit clickedBar(ids);
    }
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
