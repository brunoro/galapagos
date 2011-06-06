#include "def.h"

Def::Def()
{
    qDebug() << "Def::Def constructor" ;
}

void Def::addOperators(QString opstr){
    
    qDebug() << "Def::addOperators opstr" ;

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
}

void Def::addTerms(QString termstr){

    qDebug() << "Def::addTerms - terms " << termstr;
    
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
    
    //TODO: burn after read!
    /*From documentation ( http://doc.qt.nokia.com/4.7/qhash.html )
    In general, we recommend that you use contains() and value() rather than operator[]() for looking up a key in a hash. The reason is that operator[]() silently inserts an item into the hash if no item exists with the same key (unless the hash is const). For example, the following code snippet will create 1000 items in memory:
    */

    return ops.contains(str) ? ops.value(str) : 0;  //assuming that 0 is a good default value. TODO: is 0 a good default value brunoro?
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

