/* the implement of statement.h */
#include "program.h"
#include "mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

void MainWindow::rem_handler(Statement sta) {
    QRegExp rem_patterin("\\s*\\d+\\s+REM\\s+(.*)");
    rem_pattern.indexIn(statement);
    exp = NULL;
}

void MainWindow::let_handler(Statement sta) {
    QRegExp let_pattern("\\s*\\d+\\s+LET\\s+(\\w+)\\s*=\\s*(.*)");
    let_pattern.indexIn(statement);
    Expression *var = new IdentifierExp(let_pattern.cap(1));
    //exp = new CompoundExp("=", var);
}

void MainWindow::print_handler(Statement sta) {
    QRegExp print_pattern("\\s*\\d+\\s+PRINT\\s+(\\w+)");
    print_pattern.indexIn(statement);
    exp = NULL;
    // print the value
}

void MainWindow::input_handler(Statement sta) {
    QRegExp input_pattern("\\s*\\d+\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(statement);
    QString var = input_pattern.cap(1);
    // get input from the command line
    get_value = 1; // set flag to get value
//    to_get_input(exp);
    while(get_value){} // wait for the input
    //pro_data.setValue(exp, input_value.toInt());
}

void MainWindow::goto_handler(Statement sta) {
    QRegExp goto_pattern("\\s*\\d+\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(statement);
    exp = new ConstantExp(goto_pattern.cap(1).toInt());
}

void MainWindow::ifthen_handler(Statement sta) {
    QRegExp if_pattern("\\s*\\d+\\s+IF\\s+(.*)?THEN\\s+(\\d+)");
    if_pattern.indexIn(statement);
    //exp = if_pattern.cap(1);
//    int line_number = if_pattern.cap(2).toInt();
}

void MainWindow::end_handler(Statement sta) {
    exp = NULL;
}