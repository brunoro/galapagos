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

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QScrollBar>
#include <QWidget>

/* extends QGraphicsView to implement panning and zooming */

class Viewport : public QGraphicsView
{
    Q_OBJECT

    public:
        Viewport(QGraphicsScene *scene, QWidget *parent);

    protected:
        QPointF currentCenterPoint;
        QPoint lastPanPoint;

        virtual void wheelEvent(QWheelEvent* event);
        virtual void resizeEvent(QResizeEvent* event);

    signals:
        void scaleView(qreal factor);
};

#endif
