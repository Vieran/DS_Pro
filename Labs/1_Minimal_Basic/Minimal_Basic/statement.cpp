/* the implement of statement.h */
#include "statement.h"
#include "program.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

void Statement::rem_handler() {
    // handle rem
}

void Statement::let_handler() {
    sta = LET;
    QRegExp let_pattern("\\d+\\s+LET\\s+(\\w+)\\s*=\\s*(.*)");
    let_pattern.indexIn(origin);
    QString var = let_pattern.cap(1);
    QString exp = let_pattern.cap(2);
//    data.insert(var, exp);
}

void Statement::print_handler() {
    sta = PRINT;
    QRegExp print_pattern("\\d+\\s+PRINT\\s+(\\w+)");
    print_pattern.indexIn(origin);
    QString var = print_pattern.cap(1);
//    QString get_val(data.value(var));
//   ui->RESULT_text->append(data.value(var));
}

void Statement::input_handler() {
    sta = INPUT;
    QRegExp input_pattern("\\d+\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(origin);
    QString var = input_pattern.cap(1);
}

void Statement::goto_handler() {
    QRegExp goto_pattern("\\d+\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(origin);
    int line_number = goto_pattern.cap(1).toInt();
}

void Statement::ifthen_handler() {
    QRegExp if_pattern("\\d+\\s+IF\\s+(.*)?THEN\\s+(\\d+)");
    if_pattern.indexIn(origin);
    QString if_exp = if_pattern.cap(1);
    int line_number = if_pattern.cap(2).toInt();
}

void Statement::end_handler() {
    // handle end
}

Statement::Statement(QString in_str) {
    origin = in_str;
    QRegExp reg_exp("(\\d+)\\s+(REM|LET|PRINT|INPUT|GOTO|IF|END).*");
    reg_exp.indexIn(in_str);
    QString statement = reg_exp.cap(2);
//        qDebug() << exp.cap(2);
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