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

namespace Style
{
    /* vars */
    const float pi = 3.14159265;

    /* sizes and distances */
    const QSizeF nodeSize = QSizeF(40, 40);
    const int edgeDistance = 5;
    const int edgeWeight = 2;
    const int defaultStep = 50;
    const int paletteS = 200,
              paletteV = 200;

    const int refboxLineLen = 30;
    const int refboxPadding = 10;

    const int histogramPadding = 0;
    const int histogramWidth = 250;
    const int histogramHeight = 400;

    const int textDistance = 6;

    const int nodeZValue = 1;
    const int edgeZValue = 0;
    const int refboxZValue = 2;
    /* colors */
    /*const QColor nodeBrushColor[4] = { QColor("dodgerblue"),
                                       QColor("lightseagreen"),
                                       QColor("orange"),
                                       QColor("darkgrey") };*/

    const QPalette systemColors = QApplication::palette();

    const QColor defaultNodeColor = QColor("gainsboro");
    const QColor nodeBrushColor[4] = { QColor(defaultNodeColor),
                                       QColor(defaultNodeColor),
                                       QColor(defaultNodeColor),
                                       QColor(defaultNodeColor) };
    const QColor nodePenColor(0, 0, 0, 0);
    const QColor nodeTextColor("grey");
    const QColor edgeColor("royalblue");

    const QBrush refboxBGColor = QColor("lightgrey");
    const QPen refboxBorder(Qt::NoPen);

    const QBrush histogramBarBrush = QColor("gainsboro");
    const QBrush histogramSelectedBarBrush = QColor("royalblue");
    //const QPen histogramBarPen(Qt::NoPen);
    const QPen histogramBarPen(QColor("darkgrey"), 0.5);
    const QPen histogramAxisColor(QColor("black"), 0.5);

    /* palette functions */
    QList<QColor> getColorPalette(int len);
    QList<Qt::PenStyle> getPenStylePalette(int len);
}

#endif // STYLE_H
