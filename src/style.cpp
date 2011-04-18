#include "style.h"

Style::Style()
{
}

QHash<int, QColor> Style::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int h = 190,
        s = 190,
        v = 0;
    float ratio = 255/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        QColor color;
        v += ratio;
        palette[i] = color.fromHsv(h, s, v);
    }
    return palette;
}
