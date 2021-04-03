/* statement construction
 * expression declaration
 * parsing and evaluating
 */

#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>
#include "exp.h"

enum STA{REM, LET, PRINT, INPUT, GOTO, IF_THEN, END};
enum OP{equal, greater, less, add, sub, multi, divide, left_parenthesis, right_parenthesis}; // =><+-*/()

class Statement {
private:
    STA sta;
    Expression *exp;

    void rem_handler();
    void let_handler();
    void print_handler();
    void input_handler();
    void goto_handler();
    void ifthen_handler();
    void end_handler();
public:
    QString origin;
    Statement(QString in_str);
    ~Statement();
};

#endif //STATEMENT_H
