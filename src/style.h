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

    /* palette functions */
    QList<QColor> getColorPalette(int len);
    QList<Qt::PenStyle> getPenStylePalette(int len);
}

#endif // STYLE_H
