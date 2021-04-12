#include "program.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QtDebug>

void MainWindow::rem_dis(Statement *sta) {
    QRegExp rem_pattern("\\s*(\\d+)\\s+REM\\s+(.*)");
    rem_pattern.indexIn(sta->statement);
    QString purify_sta = rem_pattern.cap(2);
    ui->GRAMMAR_text->append(rem_pattern.cap(1) + " REM");
    ui->GRAMMAR_text->append("    " + purify_sta);
}

void MainWindow::end_dis(Statement *sta) {
    QRegExp end_pattern("\\s*(\\d+)\\s+END\\s*");
    end_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(end_pattern.cap(1) + " END");
}

void MainWindow::let_dis(Statement *sta) {
    QRegExp let_pattern("\\s*(\\d+)\\s+LET\\s+(.*)");
    let_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(let_pattern.cap(1) + " LET =");
    QString indent  = "";
    display_exp(indent, sta->exp_tree);
}

void MainWindow::print_dis(Statement *sta) {
    QRegExp print_pattern("\\s*(\\d+)\\s+PRINT\\s+(\\w+)");
    print_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(print_pattern.cap(1) + " PRINT");
    QString indent  = "";
    display_exp(indent, sta->exp_tree);
}

void MainWindow::input_dis(Statement *sta) {
    QRegExp input_pattern("\\s*(\\d+)\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(input_pattern.cap(1) + " INPUT");
    ui->GRAMMAR_text->append("    " + input_pattern.cap(2));
}

void MainWindow::ifthen_dis(Statement *sta) {
    QRegExp if_pattern("\\s*(\\d+)\\s+IF\\s+(.*)?\\s+THEN\\s+(\\d+)");
    if_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(if_pattern.cap(1) + " IF THEN");
    QString indent  = "";
    display_exp(indent, sta->exp_tree);
    ui->GRAMMAR_text->append("    " + if_pattern.cap(3));
}

void MainWindow::goto_dis(Statement *sta) {
    QRegExp goto_pattern("\\s*(\\d+)\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(goto_pattern.cap(1) + " GOTO");
    ui->GRAMMAR_text->append("    " + goto_pattern.cap(2));
}