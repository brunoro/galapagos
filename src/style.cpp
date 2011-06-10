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
