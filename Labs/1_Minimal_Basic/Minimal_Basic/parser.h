#ifndef PARSER_H
#define PARSER_H

#include "exp.h"
#include "program.h"
// functions to parse a given expression

ExpressionType type_of_exp(QString in_str) {
    QRegExp clear_exp("\\s*(.*)\\s*");
    clear_exp.indexIn(in_str);
    QString clear_str = clear_exp.cap(1); // string with no blank at the begin and end
    if (1)
        return CONSTANT;
    else if (0)
        return IDENTIFIER;
    else
        return COMPOUND;

}

#endif // PARSER_H