#ifndef STYLE_H
#define STYLE_H

#include <QColor>
#include <QHash>
#include <QList>

class Style
{
public:
    Style();
    static QHash<int, QColor> getColorPalette(QList<int> ids);
};

#endif // STYLE_H
