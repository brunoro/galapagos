#ifndef DEF_H 
#define DEF_H

#include <QHash>
#include <QRegExp>
#include <QStringList>

class Def
{
    public:
        Def(QString ops, QString vars, QString terms);

        int isOp(QString str);
        bool isVar(QString str);
        bool isTerm(QString str);
        
    private:
        QHash<QString, int> ops;
        QStringList vars;
        QStringList terms;
};

#endif
