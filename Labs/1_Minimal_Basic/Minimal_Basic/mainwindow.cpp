#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_LOAD_clicked()
{
   // on click load button
    QString getstring = ui->CODE_text->toPlainText();
    ui->RESULT_text->setText(getstring);
}

void MainWindow::on_RUN_clicked()
{
   // on click run button
}

void MainWindow::on_CLEAR_clicked()
{
   // on click clear button
    ui->CODE_text->clear();
    ui->RESULT_text->clear();
    ui->GRAMMAR_text->clear();
    ui->COMMAND_text->clear();
}

void MainWindow::on_lineEdit_returnPressed()
{
   // press enter in command input box
}
