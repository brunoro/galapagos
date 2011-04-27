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

    /* colors */
    QHash<int, QColor> getColorPalette(QList<int> ids);
}

#endif // STYLE_H
