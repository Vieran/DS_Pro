#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include <QMap>
void error(QString error_msg);

enum STA{REM, LET, PRINT, INPUT, GOTO, IF_THEN, END};
enum OP{EQUAL, ADD, SUB, MULTI, DIVIDE, LEFT_PARENTHESIS, RIGHT_PARENTHESIS, NUMBER, VALUE, ENDOFSTR}; // =+-*/^()number value eof

struct Statement {
    STA sta_type;
    QString statement;
    Expression *exp_tree;
    Statement(QString in_str) {
        statement = in_str;
        QRegExp reg_exp("\\s*(\\d+)\\s+(REM|LET|PRINT|INPUT|GOTO|IF|END)(.*)");
        reg_exp.indexIn(in_str);
        QString sta_t = reg_exp.cap(2);
        if (sta_t == "REM")
            sta_type = REM;
        else if (sta_t == "LET")
            sta_type = LET;
        else if (sta_t == "PRINT")
            sta_type = PRINT;
        else if (sta_t == "INPUT")
            sta_type = INPUT;
        else if (sta_t == "GOTO")
            sta_type = GOTO;
        else if (sta_t == "IF")
            sta_type = IF_THEN;
        else if (sta_t == "END")
            sta_type = END;
        else // error occurs
            error("error in statement construct");
        }
};


#endif // PROGRAM_H