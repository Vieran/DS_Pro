#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include <QMap>

enum STA{REM, LET, PRINT, INPUT, GOTO, IF_THEN, END};
enum OP{EQUAL, ADD, SUB, MULTI, DIVIDE, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, NUMBER, VALUE, ENDOFSTR}; // =+-*/^()number value eof

struct Statement {
    STA sta_type;
    QString statement;
    Expression *exp_tree;
    void clear_expt(Expression *expt);
    Statement(QString in_str);
    ~Statement() {
        clear_expt(exp_tree);
    }
};

#endif // PROGRAM_H