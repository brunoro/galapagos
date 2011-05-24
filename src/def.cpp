#include "def.h"

Def::Def(QString opstr, QString termstr)
{
    /* iterates on operations on the form "(name,degree) (name2,degree2)" */
    QStringList oplist = opstr.split(QRegExp("\\s+")).filter(""),
                opturn; 

    foreach(QString opiter, oplist)
    {
        if(opiter == "") break;
        opturn = opiter.remove(QRegExp("\\(|\\)")).split(",");
        //qDebug() << "Def::Def adding op" << opturn;
        ops[opturn[0]] = opturn[1].toInt();
    }

    /* iterates on terminals on the form "term1 term2 term3" */
    QStringList termlist = termstr.split(QRegExp("\\s+"));

    foreach(const QString &termiter, termlist)
    {
        if(termiter == "") break;
        //qDebug() << "Def::Def adding term" << termiter;
        terms << termiter;
    }
}

/* returns the degree of an operation if it exists, otherwise returns 0 */
int Def::isOp(QString str)
{
    //qDebug() << "Def::isOp" << str << ops[str];
    return ops[str];
}

bool Def::isTerm(QString str)
{
    //qDebug() << "Def::isTerm" << str << terms.indexOf(str);
    return (terms.indexOf(str) >= 0) ? true : false;
}

QHashIterator<QString, int> Def::getOpIterator()
{
    QHashIterator<QString, int> iter(ops);
    return iter;
}
