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

Viewport::Viewport(QGraphicsScene *scene)
    :QGraphicsView(scene)
{
    return;
}
 
/* sets the center of the screen */
void Viewport::setCenter(QPointF centerPoint) {
    /* get the rectangle of the visible area in scene coords */
    QRectF visibleArea = mapToScene(rect()).boundingRect();
 
    /* get the scene area */
    QRectF sceneBounds = sceneRect();
 
    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;
 
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);
 
    if(bounds.contains(centerPoint))
    {
        currentCenterPoint = centerPoint;
    }
    else 
    {
        if(visibleArea.contains(sceneBounds))
        {
            currentCenterPoint = sceneBounds.center();
        }
        else
        {
            currentCenterPoint = centerPoint;
 
            /* check if center point is within the borders */;
            if(centerPoint.x() > bounds.x() + bounds.width())
            {
                currentCenterPoint.setX(bounds.x() + bounds.width());
            } 
            else if(centerPoint.x() < bounds.x())
            {
                currentCenterPoint.setX(bounds.x());
            }
 
            if(centerPoint.y() > bounds.y() + bounds.height())
            {
                currentCenterPoint.setY(bounds.y() + bounds.height());
            } 
            else if(centerPoint.y() < bounds.y()) 
            {
                currentCenterPoint.setY(bounds.y());
            }
 
        }
    }
 
    /* update the scrollbars */
    centerOn(currentCenterPoint);
}

/* returns center point */
QPointF Viewport::getCenter()
{ 
    return currentCenterPoint;
}

void Viewport::wheelEvent(QWheelEvent* event) {
 
    /* get the position of the mouse before scaling, in scene coords */
    QPointF pointBeforeScale(mapToScene(event->pos()));
 
    /* get the original screen centerpoint */
    QPointF screenCenter = getCenter(); //currentCenterPoint; //(visRect.center());
 
    /* scale */
    double scaleFactor = 1.15; // zoom speed
    if(event->delta() > 0)
    {
        /* zoom in */
        scale(scaleFactor, scaleFactor);
    }
    else
    {
        /* zoom out */
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }
 
    /* get the position after scaling, in scene coords */
    QPointF pointAfterScale(mapToScene(event->pos()));
 
    /* get the delta of the screen movement */
    QPointF offset = pointBeforeScale - pointAfterScale;
 
    /* Adjust to the new center for correct zooming */
    QPointF newCenter = screenCenter + offset;
    setCenter(newCenter);
}
 
void Viewport::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    setCenter(visibleArea.center());
 
    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

