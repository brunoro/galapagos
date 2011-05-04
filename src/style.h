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

    /* colors */
    const QColor nodeBrushColor[4] = { QColor("dodgerblue"),
                                       QColor("lightseagreen"),
                                       QColor("orange"),
                                       QColor("darkgrey") };
    const QColor nodePenColor(0, 0, 0, 0);
    const QColor nodeTextColor("white");
    const QColor edgeColor("darkgrey");

    QHash<int, QColor> getColorPalette(QList<int> ids);
    QHash<int, Qt::PenStyle> getPenStylePalette(QList<int> ids);
}

#endif // STYLE_H
