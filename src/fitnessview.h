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

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QList>
#include <QObject>

#include <cmath>
#include <climits>

#include "style.h"
#include "generation.h"

#define DEFAULT_HISTOGRAM_SIZE 10
#define NO_SLICE INT_MAX

struct idfit
{
    int id;
    float fit;
};

bool fitnessLessThan(const idfit &a, const idfit &b);

class HistogramBar: public QGraphicsRectItem
{
    public:
        HistogramBar(qreal x, qreal y, qreal width, qreal height, QList<int> nids, int nnum);
        void select();
        void deselect();
        QList<int> getIds();
        int getNum();
    private:
        int num;
        QList<int> ids;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
};

class Histogram: public QGraphicsScene
{
    Q_OBJECT

    public:
        Histogram(qreal x, qreal y, qreal width, qreal height, QWidget *parent);
        void setSize(int size);
        void setData(Generation *gen);
        void draw();
        QList<int> getSlice(int slice);
        void selectSlice(int slice);
        int getSelectedSlice();
    private:
        int size;
        Generation *data;
        QList<HistogramBar*> bars;
        int selectedSlice;
    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    signals:
        void clickedBar();
};
