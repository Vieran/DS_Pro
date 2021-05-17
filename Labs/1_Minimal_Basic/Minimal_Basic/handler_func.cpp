#include "program.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "parser.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

// construct let statement
void MainWindow::let_handler(Statement *sta) {
    QRegExp let_pattern("\\s*\\d+\\s+LET\\s+(.*)");
    let_pattern.indexIn(sta->statement);
    QString purify_sta = let_pattern.cap(1);
    try {
        construct_exp_tree(sta, purify_sta);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// construct print statement
void MainWindow::print_handler(Statement *sta) {
    QRegExp print_pattern("\\s*\\d+\\s+PRINT\\s+(.*)");
    print_pattern.indexIn(sta->statement);
    QString purify_sta = print_pattern.cap(1);
    try {
        construct_exp_tree(sta, purify_sta);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// construct printf statement
void MainWindow::printf_handler(Statement *sta) {
    // this function do nothing, execute function will do everything
}

// construct if_then statement
void MainWindow::ifthen_handler(Statement *sta) {
    QRegExp if_pattern("\\s*\\d+\\s+IF\\s+(.*)?\\s+THEN(.*)");
    if_pattern.indexIn(sta->statement);
    QString purify_sta = if_pattern.cap(1);
    try {
        construct_exp_tree_ifthen(sta, purify_sta);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}