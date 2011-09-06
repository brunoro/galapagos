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

#include "style.h"

/* default constructor */
Style::Style()
{
    /* sizes and distances */
    windowWidth = 1024;
    windowHeight= 800;

    sceneWidth = 1500;
    sceneHeight= 1500;

    nodeSize = QSizeF(40, 40);
    edgeDistance = 5;
    edgeWeight = 2;
    defaultStep = 50;
    
    refboxLineLen = 30;
    refboxPadding = 10;
    refboxBorderRadius = 5;

    histogramPadding = 0;
    histogramWidth = 250;
    histogramHeight = 400;

    textDistance = 6;

    nodeZValue = 1;
    edgeZValue = 0;
    refboxZValue = 2;

    /* hue and saturation for palette */
    paletteS = 200;
    paletteV = 200;

    /* angles for palette */
    paletteRange = 360;
    paletteStart = 0;

    /* colors */
    defaultNodeColor = QColor("gainsboro");
    nodePenColor = QColor(0, 0, 0, 0);
    nodeTextColor = QColor("grey");
    edgeColor = QColor("royalblue");

    ringColor = QColor(Qt::gray);

    refboxBGColor = QBrush(QColor("lightgrey"));
    refboxBorder = QPen(Qt::NoPen);

    histogramBarBrush = QBrush(QColor("gainsboro"));
    histogramSelectedBarBrush = QBrush(QColor("royalblue"));
    histogramBarPen = QPen(QColor("darkgrey"), 0.5);
    histogramAxisColor = QPen(QColor("black"), 0.5);
}

QList<QColor> Style::getColorPalette(int len)
{
    QList<QColor> palette;
    if(len > 0)
    {
        int h = paletteStart,
            s = paletteS,
            v = paletteV;
        float ratio = paletteRange / len;
        for(int i = 0; i < len; i++)
        {
            palette.append(QColor::fromHsv(h, s, v));
            h += ratio;
        }
    }
    return palette;
}

// TODO: make it work for more than 6 trees
QList<Qt::PenStyle> Style::getPenStylePalette(int len)
{
    QList<Qt::PenStyle> palette;
    for(int i = 0; i < len; i++)
        palette.append((Qt::PenStyle) (i + 1));

    return palette;
}
