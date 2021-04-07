#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include <QMap>
void error(QString error_msg);

enum STA{REM, LET, PRINT, INPUT, GOTO, IF_THEN, END};
enum OP{equal, greater, less, add, sub, multi, divide, left_parenthesis, right_parenthesis}; // =><+-*/()

struct Statement {
    STA sta_type;
    QString statement;
    Statement(QString in_str) {
        statement = in_str;
        QRegExp reg_exp("\\s*(\\d+)\\s+(REM|LET|PRINT|INPUT|GOTO|IF|END).*");
        reg_exp.indexIn(in_str);
        QString statement = reg_exp.cap(2);
        if (statement == "REM")
            sta_type = REM;
        else if (statement == "LET")
            sta_type = LET;
        else if (statement == "PRINT")
            sta_type = PRINT;
        else if (statement == "INPUT")
            sta_type = INPUT;
        else if (statement == "GOTO")
            sta_type = GOTO;
        else if (statement == "IF")
            sta_type = IF_THEN;
        else if (statement == "END")
            sta_type = END;
        else // error occurs
            error("error in statement construct");
        }
};


#endif // PROGRAM_H