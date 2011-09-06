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

#ifndef STYLE_H
#define STYLE_H

#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QSizeF>
#include <QPalette>
#include <QPen>

class Style
{
    public:
        Style();

        /* vars */
        static const float pi = 3.14159265;

        /* sizes and distances */
        int windowWidth;
        int windowHeight;

        int sceneWidth;
        int sceneHeight;

        QSizeF nodeSize;
        int edgeDistance;
        int edgeWeight;
        int defaultStep;
        int paletteS;
        int paletteV;
        int paletteStart;
        int paletteRange;

        int refboxLineLen;
        int refboxPadding;
        int refboxBorderRadius;

        int histogramPadding;
        int histogramWidth;
        int histogramHeight;

        int textDistance;

        int nodeZValue;
        int edgeZValue;
        int refboxZValue;

        /* colors */
        QColor defaultNodeColor;
        QColor nodePenColor;
        QColor nodeTextColor;
        QColor edgeColor;

        QColor ringColor;

        QBrush refboxBGColor;
        QPen refboxBorder;

        QBrush histogramBarBrush;
        QBrush histogramSelectedBarBrush;
        QPen histogramBarPen;
        QPen histogramAxisColor;

        /* palette functions */
        QList<QColor> getColorPalette(int len);
        QList<Qt::PenStyle> getPenStylePalette(int len);
};

#endif // STYLE_H
