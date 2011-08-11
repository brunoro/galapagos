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

#include "viewport.h"

Viewport::Viewport(QGraphicsScene *scene, QWidget *parent)
    :QGraphicsView(scene, parent)
{
    return;
}

void Viewport::wheelEvent(QWheelEvent* event)
{
 
    /* get the position of the mouse before scaling, in scene coords */
    QPointF pointBeforeScale(mapToScene(event->pos()));
 
    /* get the original screen centerpoint */
    //QPointF screenCenter = getCenter(); //currentCenterPoint; //(visRect.center());
 
    /* scale */
    double scaleFactor = 1.15; // zoom speed
    if(event->delta() > 0)
    {
        /* zoom in */
        scale(scaleFactor, scaleFactor);
        emit scaleView(1.0/scaleFactor);
    }
    else
    {
        /* zoom out */
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
        emit scaleView(scaleFactor);
    }
 
    /* get the position after scaling, in scene coords */
    //QPointF pointAfterScale(mapToScene(event->pos()));
 
    /* get the delta of the screen movement */
    //QPointF offset = pointBeforeScale - pointAfterScale;
 
    /* Adjust to the new center for correct zooming */
    //QPointF newCenter = screenCenter + offset;
    //setCenter(newCenter);
}
 
void Viewport::resizeEvent(QResizeEvent* event)
{
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
//    setCenter(visibleArea.center());
 
    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

