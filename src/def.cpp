/*
 * Galapagos - a genetic programming algorithms visualizer
 * http://github.com/brunoro/galapagos
 * Copyright (C) <2011>  <Gustavo Brunoro>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "def.h"

Def::Def()
{
    //qDebug() << "Def::Def constructor" ;
}

void Def::addOperators(QString opstr){
    
    //qDebug() << "Def::addOperators opstr" ;

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

    //qDebug() << "Def::addTerms - terms " << termstr;
    
    /* iterates on terminals on the form "term1 term2 term3" */
    QStringList termlist = termstr.split(QRegExp("\\s+"));

    foreach(const QString &termiter, termlist)
    {
        if(termiter == "") break;
        ////qDebug() << "Def::Def adding term" << termiter;
        terms << termiter;
    }
}

/* returns the degree of an operation if it exists, otherwise returns 0 */
int Def::isOp(QString str)
{
    //qDebug() << "Def::isOp" << str << ops.value(str);
    return ops.contains(str) ? ops.value(str) : 0;
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

