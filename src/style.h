#ifndef STYLE_H
#define STYLE_H

#include <QColor>
#include <QDebug>
#include <QHash>
#include <QList>
#include <QSizeF>

namespace Style
{
    /* vars */
    const float pi = 3.14159265;

    /* sizes and distances */
    const QSizeF nodeSize = QSizeF(35, 35);
    const int edgeDistance = 5;
    const int edgeWeight = 2;
    const int defaultStep = 50;
    const int paletteS = 200,
              paletteV = 200;

    /* colors */
    /*const QColor nodeBrushColor[4] = { QColor("dodgerblue"),
                                       QColor("lightseagreen"),
                                       QColor("orange"),
                                       QColor("darkgrey") };*/

    const QColor defaultNodeColor = QColor("dimgrey");
    const QColor nodeBrushColor[4] = { QColor(defaultNodeColor),
                                       QColor(defaultNodeColor),
                                       QColor(defaultNodeColor),
                                       QColor(defaultNodeColor) };
    const QColor nodePenColor(0, 0, 0, 0);
    const QColor nodeTextColor("white");
    const QColor edgeColor("orange");

    QList<QColor> getColorPalette(int len);
    QList<Qt::PenStyle> getPenStylePalette(int len);
}

#endif // STYLE_H
