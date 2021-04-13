#ifndef PARSER_H
#define PARSER_H

#include "exp.h"
#include "program.h"
#include <QDebug>

ExpressionType type_of_exp(QString in_str);
void construct_exp_tree(Statement *sta, QString purify_sta, int lmr=0);
void construct_exp_tree_ifthen(Statement *sta, QString purify_sta);

#endif // PARSER_H