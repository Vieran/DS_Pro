/* statement construction
 * expression declaration
 * parsing and evaluating
 */

#ifndef STATEMENT_H
#define STATEMENT_H

#include <QString>

enum STA{REM, LET, PRINT, INPUT, GOTO, IF, END};
enum OP{equal, greater, less, add, sub, multi, divide, left_parenthesis, right_parenthesis}; // =><+-*/()

class Statement {
private:
    QString origin;
    STA sta;
    QString *exp_tree;

    void rem_handler();
    void let_handler();
    void print_handler();
    void input_handler();
    void goto_handler();
    void ifthen_handler();
    void end_handler();
public:
    Statement(QString in_str);
};

#endif //STATEMENT_H
