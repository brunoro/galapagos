#ifndef DEF_H 
#define DEF_H

#include <QDebug>
#include <QHash>
#include <QHashIterator>
#include <QRegExp>
#include <QStringList>

class Def
{
    public:
        Def();
        void addOperators(QString opstr);
        void addTerms(QString terms);

        int isOp(QString str);
        bool isTerm(QString str);

        QHashIterator<QString, int> getOpIterator();

    private:
        QHash<QString, int> ops;
        QStringList terms;
};

#endif
