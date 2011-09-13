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

void Def::addNodes(QString opstr){
    
    //qDebug() << "Def::addOperators opstr" ;

    /* iterates on operations on the form "(name,degree) (name2,degree2)" */
    QStringList nodelist = opstr.split(QRegExp("\\s+")).filter(""),
                nodeturn; 

    foreach(QString nodeiter, nodelist)
    {
        if(nodeiter == "") break;
        nodeturn = nodeiter.remove(QRegExp("\\(|\\)")).split(",");
        //qDebug() << "Def::Def adding node" << nodeturn;
        int degree = nodeturn[1].toInt();
        nodes[nodeturn[0]] = degree;
        if(degree > 0)
            ops[nodeturn[0]] = degree;
    }
}

/* returns the degree of an operation if it exists, otherwise returns NOT_A_NODE */
int Def::nodeDegree(QString str)
{
    //qDebug() << "Def::isNode" << str << ops.value(str);
    return nodes.contains(str) ? ops.value(str) : NOT_A_NODE;
}

QHashIterator<QString, int> Def::getOpIterator()
{
    QHashIterator<QString, int> iter(ops);
    return iter;
}

