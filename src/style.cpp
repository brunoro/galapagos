#include "style.h"

Style::Style()
{
}

QHash<int, QColor> Style::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int v = 190,
        s = 190,
        h = 0;
    float ratio = 255/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        QColor color(0, 0, 0);
        h += ratio;
        palette[i] = color.fromHsv(h, s, v);
    }
    return palette;
}
