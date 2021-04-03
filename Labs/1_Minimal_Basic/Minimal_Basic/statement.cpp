/* the implement of statement.h */
#include "statement.h"
#include "program.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

void Statement::rem_handler() {
    sta = REM;
    QRegExp rem_pattern("\\s*\\d+\\s+REM\\s+(.*)");
    rem_pattern.indexIn(origin);
    exp = NULL;
}

void Statement::let_handler() {
    sta = LET;
    QRegExp let_pattern("\\s*\\d+\\s+LET\\s+(\\w+)\\s*=\\s*(.*)");
    let_pattern.indexIn(origin);
    Expression *var = new IdentifierExp(let_pattern.cap(1));
    //exp = new CompoundExp("=", var);
}

void Statement::print_handler() {
    sta = PRINT;
    QRegExp print_pattern("\\s*\\d+\\s+PRINT\\s+(\\w+)");
    print_pattern.indexIn(origin);
    exp = NULL;
    // print the value
}

void Statement::input_handler() {
    sta = INPUT;
    QRegExp input_pattern("\\s*\\d+\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(origin);
    QString var = input_pattern.cap(1);
    // get input from the command line
    get_value = 1; // set flag to get value
//    to_get_input(exp);
    while(get_value){} // wait for the input
    //pro_data.setValue(exp, input_value.toInt());
}

void Statement::goto_handler() {
    sta = GOTO;
    QRegExp goto_pattern("\\s*\\d+\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(origin);
    exp = new ConstantExp(goto_pattern.cap(1).toInt());
}

void Statement::ifthen_handler() {
    sta = IF_THEN;
    QRegExp if_pattern("\\s*\\d+\\s+IF\\s+(.*)?THEN\\s+(\\d+)");
    if_pattern.indexIn(origin);
    //exp = if_pattern.cap(1);
//    int line_number = if_pattern.cap(2).toInt();
}

void Statement::end_handler() {
    sta = END;
    exp = NULL;
}

Statement::Statement(QString in_str) {
    origin = in_str;
    qDebug() << origin;
    QRegExp reg_exp("\\s*(\\d+)\\s+(REM|LET|PRINT|INPUT|GOTO|IF|END).*");
    reg_exp.indexIn(in_str);
    QString statement = reg_exp.cap(2);
    if (statement == "REM")
        rem_handler();
    else if (statement == "LET")
        let_handler();
    else if (statement == "PRINT")
        print_handler();
    else if (statement == "INPUT")
        input_handler();
    else if (statement == "GOTO")
        goto_handler();
    else if (statement == "IF")
        ifthen_handler();
    else if (statement == "END")
        end_handler();
    else // error occurs
        error("error in statement construct");
}

Statement::~Statement() {
    if (exp != NULL)
        delete exp;
}