#include "fitnessview.h"

/* Histogram */
Histogram::Histogram(qreal x, qreal y, qreal width, qreal height, QWidget *parent)
    :QGraphicsScene(x, y, width, height, (QObject*)parent)
{
    size = DEFAULT_HISTOGRAM_SIZE;
    data = NULL;
    selectedSlice = NO_SLICE;
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
    /* assuming minimum fitness 0 and maximum 100
     * TODO: move this to definition file */
    //float smallest = pairs.first().fit,
    //      bigger = pairs.last().fit;
    float smallest = 0.0,
          bigger = 100.0;
    
    float bucketSize = 1.0 / size;
    QVector< QList<int> > buckets(size);

    int bucketCount = 0;
    for(int i = 0; i < pairs.length(); i++)
    {
        pairs[i].fit = ((pairs[i].fit - smallest) / bigger);
        if(pairs[i].fit > bucketCount * bucketSize)
            if(bucketCount < size - 1)
                bucketCount++;
        buckets[bucketCount].append(pairs[i].id);
    }

    /* draw bars */
    bars.clear();
    for(int i = 0; i < size; i++)
    {
        bars.append(new HistogramBar(Style::histogramPadding,
                                     Style::histogramHeight/size * i,
                                     Style::histogramWidth * buckets[i].length() / pairs.length(),
                                     Style::histogramHeight / size,
                                     buckets[i], i));
        /* labels on fitness axis */
        QGraphicsSimpleTextItem *ylabel = new QGraphicsSimpleTextItem(QString::number(i * bigger / size));
        ylabel->setPos(Style::histogramPadding - ylabel->boundingRect().width() - Style::textDistance,
                       Style::histogramHeight / size * i - ylabel->boundingRect().height() / 2);
        addItem(ylabel);

        /* tooltips on bars */
        if(buckets[i].length() <= 1)
            bars[i]->setToolTip(QString::number(buckets[i].length()) + " individual");
        else
            bars[i]->setToolTip(QString::number(buckets[i].length()) + " individuals");
        addItem(bars[i]);
    }

    /* draw axis */
    QGraphicsLineItem *xaxis = new QGraphicsLineItem(Style::histogramPadding, Style::histogramHeight,
                                                     Style::histogramWidth, Style::histogramHeight);
    QGraphicsLineItem *yaxis = new QGraphicsLineItem(Style::histogramPadding, 0,
                                                     Style::histogramPadding, Style::histogramHeight);

    xaxis->setPen(Style::histogramAxisColor);
    yaxis->setPen(Style::histogramAxisColor);
    addItem(xaxis);
    addItem(yaxis);
    
    /* max y label */
    QGraphicsSimpleTextItem *ylabel = new QGraphicsSimpleTextItem(QString::number(bigger));
    ylabel->setPos(Style::histogramPadding - ylabel->boundingRect().width() - Style::textDistance,
                   - ylabel->boundingRect().height() / 2 );
    addItem(ylabel);
    
    /* y axis title */
    QGraphicsSimpleTextItem *ytitle = new QGraphicsSimpleTextItem("Fitness");
    ytitle->setPos(Style::histogramPadding - ytitle->boundingRect().width() - Style::textDistance,
                   - 2 * ytitle->boundingRect().height());
    addItem(ytitle);

    /* x labels */
    //TODO: adjust based on numbers that appear */
    int numXLabels = 5;
    for(int i = 0; i < numXLabels + 1; i++)
    {
        QGraphicsSimpleTextItem *xlabel = new QGraphicsSimpleTextItem(QString::number(i * pairs.length() / numXLabels));
        xlabel->setPos(Style::histogramPadding + i * Style::histogramWidth / numXLabels - xlabel->boundingRect().width() / 2,
                       Style::histogramHeight + Style::textDistance);
        addItem(xlabel);
    }

    /* x title */
    QGraphicsSimpleTextItem *xtitle = new QGraphicsSimpleTextItem("Number of individuals");
    xtitle->setPos(Style::histogramPadding + Style::histogramWidth - xtitle->boundingRect().width(),
                   Style::histogramHeight + Style::textDistance + xtitle->boundingRect().height());
    addItem(xtitle);
}

void Histogram::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    QGraphicsItem *item = mouseGrabberItem();
    QList<int> ids;
    if(item != NULL)
    {
        ids = ((HistogramBar*)item)->getIds();
        selectedSlice = ((HistogramBar*)item)->getNum();

        foreach(HistogramBar *bar, bars)
            bar->deselect();
        bars[selectedSlice]->select();

        emit clickedBar();
    }
}

void Histogram::selectSlice(int slice)
{
    if(slice >= size)
        return;
    selectedSlice = slice;
    foreach(HistogramBar *bar, bars)
        bar->deselect();
    bars[slice]->select();
}

QList<int> Histogram::getSlice(int slice)
{
    if(slice < size)
        return bars[slice]->getIds();
    else
        return QList<int>();
}

int Histogram::getSelectedSlice()
{
    return selectedSlice;
}

/* Histogram bar class */
HistogramBar::HistogramBar(qreal x, qreal y, qreal width, qreal height, QList<int> nids, int nnum)
    :QGraphicsRectItem(x, y, width, height)
{
    num = nnum;
    ids = nids;
    setBrush(Style::histogramBarBrush);
    setPen(Style::histogramBarPen);
}

void HistogramBar::select()
{
    setBrush(Style::histogramSelectedBarBrush);
}

void HistogramBar::deselect()
{
    setBrush(Style::histogramBarBrush);
}

QList<int> HistogramBar::getIds()
{
    return ids;
}

int HistogramBar::getNum()
{
    return num;
}

void HistogramBar::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    mouseEvent->setAccepted(true);
}
