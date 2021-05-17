/* functions to display expression tree in the GRAMMAR_tree box */
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
    display_exp(indent, sta->exp_tree->getLHS());
    display_exp(indent, sta->exp_tree->getRHS());
}

void MainWindow::print_dis(Statement *sta) {
    QRegExp print_pattern("\\s*(\\d+)\\s+PRINT\\s+(.*)");
    print_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(print_pattern.cap(1) + " PRINT");
    QString indent = "";
    display_exp(indent, sta->exp_tree);
}

void MainWindow::printf_dis(Statement *sta) {
    // just display all the statement in one line
    QRegExp printf_pattern("\\s*(\\d+)\\s+PRINTF\\s+(.*)");
    printf_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(printf_pattern.cap(1) + " PRINTF");
    ui->GRAMMAR_text->append("    " + printf_pattern.cap(2));
}

void MainWindow::input_dis(Statement *sta) {
    QRegExp input_pattern("\\s*(\\d+)\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(input_pattern.cap(1) + " INPUT");
    ui->GRAMMAR_text->append("    " + input_pattern.cap(2));
}

void MainWindow::inputs_dis(Statement *sta) {
    QRegExp inputs_pattern("\\s*(\\d+)\\s+INPUTS\\s+(\\w+)");
    inputs_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(inputs_pattern.cap(1) + " INPUTS");
    ui->GRAMMAR_text->append("    " + inputs_pattern.cap(2));
}

void MainWindow::ifthen_dis(Statement *sta) {
    QRegExp if_pattern("\\s*(\\d+)\\s+IF\\s+(.*)?\\s+THEN\\s+(\\d+)");
    if_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(if_pattern.cap(1) + " IF THEN");
    QString indent  = "";
    display_exp(indent, sta->exp_tree->getLHS());
    ui->GRAMMAR_text->append("    " + sta->exp_tree->getOperator());
    display_exp(indent, sta->exp_tree->getRHS());
    ui->GRAMMAR_text->append("    " + if_pattern.cap(3));
}

void MainWindow::goto_dis(Statement *sta) {
    QRegExp goto_pattern("\\s*(\\d+)\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(sta->statement);
    ui->GRAMMAR_text->append(goto_pattern.cap(1) + " GOTO");
    ui->GRAMMAR_text->append("    " + goto_pattern.cap(2));
}