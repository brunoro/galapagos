#include "style.h"

QHash<int, QColor> Style::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int h = 0,
        s = 200,
        v = 200;
    float ratio = 360/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        palette[ids[i]] = QColor::fromHsv(h, s, v);
        h += ratio;
    }
    return palette;
}
