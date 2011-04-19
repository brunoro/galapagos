#include "style.h"

Style::Style()
{
}

QHash<int, QColor> Style::getColorPalette(QList<int> ids)
{
    QHash<int, QColor> palette;
    int   v = 190,
          s = 190,
          h = 0;
    float ratio = 360/ids.length();
    for(int i = 0; i < ids.length(); i++)
    {
        h += ratio;
        palette[ids[i]] = QColor::fromHsv(h, s, v);
    }
    return palette;
}
