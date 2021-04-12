#include "program.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

void MainWindow::let_handler(Statement *sta) {
    QRegExp let_pattern("\\s*\\d+\\s+LET\\s+(.*)");
    let_pattern.indexIn(sta->statement);
    QString purify_sta = let_pattern.cap(1);
    construct_exp_tree(sta, purify_sta);
}

void MainWindow::print_handler(Statement *sta) {
    QRegExp print_pattern("\\s*\\d+\\s+PRINT\\s+(\\w+)");
    print_pattern.indexIn(sta->statement);
    QString purify_sta = print_pattern.cap(1);
    construct_exp_tree(sta, purify_sta);
}

void MainWindow::input_handler(Statement *sta) {
    QRegExp input_pattern("\\s*\\d+\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(sta->statement);
    input_var = input_pattern.cap(1);
    ui->COMMAND_input->setText(input_var);
    need_input_var = true;
}

void MainWindow::ifthen_handler(Statement *sta) {
    QRegExp if_pattern("\\s*\\d+\\s+IF\\s+(.*)?\\s+THEN(.*)");
    if_pattern.indexIn(sta->statement);
    QString purify_sta = if_pattern.cap(1);
    construct_exp_tree_ifthen(sta, purify_sta);
}