#include "program.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>

// traverse and execute all statement
void MainWindow::execute() {
    ui->RESULT_text->clear();
    QMap<int, Statement>::iterator tmp = basic_program.begin();
    Statement *current_sta = &(tmp.value());
    int next_line = -1;
    while (tmp != basic_program.end() && current_sta->sta_type != END && !error_occur) {
        current_sta = &(tmp.value());
        next_line = -1;
        switch (current_sta->sta_type) {
        case REM: case END: case INPUT:
            break;
        case LET:
            let_exe(current_sta);
            break;
        case PRINT:
            print_exe(current_sta);
            break;
        case IF_THEN:
            next_line = ifthen_exe(current_sta);
            break;
        case GOTO:
            next_line = goto_exe(current_sta);
            break;
        }
        if (next_line != -1)
            tmp = basic_program.find(next_line);
        else
            tmp++;
    }
}

// execute let statement
void MainWindow::let_exe(Statement *sta) {
    try {
        sta->exp_tree->eval(variable);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// execute print statememt
void MainWindow::print_exe(Statement *sta) {
    try {
        int result = sta->exp_tree->eval(variable);
        ui->RESULT_text->append(QString::number(result));
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// execute if then statement
// return the line number when true
int MainWindow::ifthen_exe(Statement *sta) {
    Expression *lhs = sta->exp_tree->getLHS();
    Expression *rhs = sta->exp_tree->getRHS();
    int left_result;
    int right_result;
    try {
        left_result = lhs->eval(variable);
        right_result = rhs->eval(variable);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
    QString op = sta->exp_tree->getOperator();

    if ((op == "=" && left_result == right_result) || (op == ">" && left_result > right_result) || (op == "<" && left_result < right_result)) {
        QRegExp if_pattern("\\s*(\\d+)\\s+IF\\s+(.*)?\\s+THEN\\s+(\\d+)");
        if_pattern.indexIn(sta->statement);
        return if_pattern.cap(3).toUInt();
    }
    return -1;
}

// execute goto statement
// return the line number
int MainWindow::goto_exe(Statement *sta) {
    QRegExp goto_pattern("\\s*\\d+\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(sta->statement);
    return goto_pattern.cap(1).toUInt();
}