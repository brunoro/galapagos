#include "def.h"

Def::Def(QString opstr, QString varstr, QString termstr)
{
    /* iterates on operations on the form "(name,degree) (name2,degree2)" */
    QStringList oplist = opstr.split(QRegExp("\\s+")).filter(""),
                opturn; 

    foreach(QString opiter, oplist)
    {
        if(opiter == "") break;
        opturn = opiter.remove(QRegExp("\\(|\\)")).split(",");
        qDebug() << "Def::Def : adding op " << opturn;
        ops[opturn[0]] = opturn[1].toInt();
    }

    /* iterates on variables on the form "var1 var2 var3" */
    QStringList varlist = varstr.split(QRegExp("\\s+"));

    foreach(const QString &variter, varlist)
    {
        if(variter == "") break;
        qDebug() << "Def::Def : adding var " << variter;
        vars << variter;
    }

    /* iterates on terminals on the form "term1 term2 term3" */
    QStringList termlist = termstr.split(QRegExp("\\s+"));

    foreach(const QString &termiter, termlist)
    {
        if(termiter == "") break;
        qDebug() << "Def::Def : adding term " << termiter;
        terms << termiter;
    }
}

/* returns the degree of an operation if it exists, otherwise returns 0 */
int Def::isOp(QString str)
{
    qDebug() << "Def::isOp : str " << str << " ops[str] " << ops[str];
    return ops[str];
}

bool Def::isVar(QString str)
{
    return (vars.indexOf(str) > 0) ? true : false;
}

bool Def::isTerm(QString str)
{
    return (terms.indexOf(str) > 0) ? true : false;
}
