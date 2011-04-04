#include "def.h"

Def::Def(QString opstr, QString varstr)
{
    /* iterates on operations on the form (name, degree) (name2, degree2) etc */
    QStringList oplist = opstr.split(" "),
                opturn; 
    QString opiter;

    foreach(opiter, oplist)
    {
        opturn = opiter.remove(QRegExp("(|)").split(","));
        ops[opturn[0]] = opturn[1].toInt();
    }

    /* iterates on variables on the form var1 var2 var3 */
    QStringList varlist = varstr.split(" ");
    QString variter;

    foreach(variter, varturn)
    {
        vars << variter;
    }
}
