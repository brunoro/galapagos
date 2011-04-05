#ifndef DEF_H 
#define DEF_H

#include <QHash>
#include <QRegExp>
#include <QStringList>

class Def:
    public:
        Def(QString ops, QString vars);
        
        QHash<QString, int> ops;
        QStringList vars;

#endif
