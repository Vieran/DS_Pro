#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include <QMap>

enum STA{REM, LET, PRINT, PRINTF, INPUT, INPUTS, GOTO, IF_THEN, END};
enum OP{EQUAL, ADD, SUB, MULTI, DIVIDE, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, NUMBER, VALUE, STR, ENDOFSTR}; // =+-*/^()number value eof
enum INPUT_TYPE{INTEGER_VAR, STRING_VAR};
struct var_in {
    QString var;
    INPUT_TYPE type;
    var_in(QString var, INPUT_TYPE type): var(var), type(type) {}
};

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