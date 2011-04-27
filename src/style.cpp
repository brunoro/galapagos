#include "style.h"

QHash<int, QColor> Style::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int h = 0,
        s = 200,
        v = 150;
    float ratio = 360/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        palette[ids[i]] = QColor::fromHsv(h, s, v);
        h += ratio;
    }
    return palette;
}

// TODO: make it work for more than 6 trees
QHash<int, Qt::PenStyle> Style::getPenStylePalette(QList<int> ids)
{
    QHash<int, Qt::PenStyle> palette;
    for(int i = 0; i < ids.length(); i++)
        palette[ids[i]] = (Qt::PenStyle) (i + 1);

    return palette;
}
