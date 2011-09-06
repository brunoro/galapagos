/*
 * Galapagos - a genetic programming algorithms visualizer
 * http://github.com/brunoro/galapagos
 * Copyright (C) <2011>  <Gustavo Brunoro>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "fitnessview.h"

extern Style* style;

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
        while( ! (pairs[i].fit > bucketCount * bucketSize && pairs[i].fit < (bucketCount+1) * bucketSize))
            if(bucketCount < size - 1)
                bucketCount++;
        
        buckets[bucketCount].append(pairs[i].id);
    }

    /* draw bars */
    bars.clear();
    for(int i = 0; i < size; i++)
    {
        bars.append(new HistogramBar(::style->histogramPadding,
                                     ::style->histogramHeight/size * (size - i - 1),
                                     ::style->histogramWidth * buckets[i].length() / pairs.length(),
                                     ::style->histogramHeight / size,
                                     buckets[i], i));
        /* labels on fitness axis */
        QGraphicsSimpleTextItem *ylabel = new QGraphicsSimpleTextItem(QString::number((size - i) * bigger / size));
        ylabel->setPos(::style->histogramPadding - ylabel->boundingRect().width() - ::style->textDistance,
                       ::style->histogramHeight / size * i - ylabel->boundingRect().height() / 2);
        addItem(ylabel);

        /* tooltips on bars */
        if(buckets[i].length() <= 1)
            bars[i]->setToolTip(QString::number(buckets[i].length()) + " individual");
        else
            bars[i]->setToolTip(QString::number(buckets[i].length()) + " individuals");
        addItem(bars[i]);
    }

    /* draw axis */
    QGraphicsLineItem *xaxis = new QGraphicsLineItem(::style->histogramPadding, ::style->histogramHeight,
                                                     ::style->histogramWidth, ::style->histogramHeight);
    QGraphicsLineItem *yaxis = new QGraphicsLineItem(::style->histogramPadding, 0,
                                                     ::style->histogramPadding, ::style->histogramHeight);

    xaxis->setPen(::style->histogramAxisColor);
    yaxis->setPen(::style->histogramAxisColor);
    addItem(xaxis);
    addItem(yaxis);
    
    /* max y label */
    QGraphicsSimpleTextItem *ylabel = new QGraphicsSimpleTextItem(QString::number(bigger));
    ylabel->setPos(::style->histogramPadding - ylabel->boundingRect().width() - ::style->textDistance,
                   - ylabel->boundingRect().height() / 2 );
    addItem(ylabel);
    
    /* y axis title */
    QGraphicsSimpleTextItem *ytitle = new QGraphicsSimpleTextItem("Fitness");
    ytitle->setPos(::style->histogramPadding - ytitle->boundingRect().width() - ::style->textDistance,
                   - 2 * ytitle->boundingRect().height());
    addItem(ytitle);

    /* x labels */
    //TODO: adjust based on numbers that appear */
    int numXLabels = 5;
    for(int i = 0; i < numXLabels + 1; i++)
    {
        QGraphicsSimpleTextItem *xlabel = new QGraphicsSimpleTextItem(QString::number(i * pairs.length() / numXLabels));
        xlabel->setPos(::style->histogramPadding + i * ::style->histogramWidth / numXLabels - xlabel->boundingRect().width() / 2,
                       ::style->histogramHeight + ::style->textDistance);
        addItem(xlabel);
    }

    /* x title */
    QGraphicsSimpleTextItem *xtitle = new QGraphicsSimpleTextItem("Number of individuals");
    xtitle->setPos(::style->histogramPadding + ::style->histogramWidth - xtitle->boundingRect().width(),
                   ::style->histogramHeight + ::style->textDistance + xtitle->boundingRect().height());
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
    setBrush(::style->histogramBarBrush);
    setPen(::style->histogramBarPen);
}

void HistogramBar::select()
{
    setBrush(::style->histogramSelectedBarBrush);
}

void HistogramBar::deselect()
{
    setBrush(::style->histogramBarBrush);
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
