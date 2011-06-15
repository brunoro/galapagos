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

QList<QColor> Style::getColorPalette(int len)
{
    QList<QColor> palette;
    if(len > 0)
    {
        int h = 0,
            s = paletteS,
            v = paletteV;
        float ratio = 360/len;
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
