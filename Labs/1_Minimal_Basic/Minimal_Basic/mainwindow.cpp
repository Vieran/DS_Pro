#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "program.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

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

// show the code in the code_text box
void MainWindow::show_code() {
    ui->CODE_text->clear(); // clear the text first
    QMap<int, Statement>::const_iterator tmp;
    tmp = basic_program.constBegin();
    while (tmp != basic_program.constEnd()) {
        ui->CODE_text->append(tmp.value().origin);
        ++tmp;
    }
}

// show help message
void MainWindow::show_help() {
    QMessageBox::information(this,
                             "HELP",
                             "USAGE: COMMAND or STATEMENT.\n"
                             "COMMANDs: LOAD, RUN, CLEAR, HELP, QUIT.\n"
                             "STATEMENT: Begin with a line number which should be a positve number.\n",
                             QMessageBox::Yes);
}

// store the input command into basic_program
void MainWindow::command_handler(QString in_str) {
    QRegExp reg_exp("\\s*(\\d+)\\s*.*");
    reg_exp.indexIn(in_str);
    int line_number = reg_exp.cap(1).toInt();
    if (line_number) {
        // if the command is only a line number, then delete that line(what if there is some blank?)
        if (in_str.length() == reg_exp.cap(1).length())
            basic_program.remove(line_number);
        else
            basic_program.insert(line_number, in_str);
    } else
        show_help();
}

void MainWindow::on_LOAD_clicked()
{
   // on click load button
    QString filename = QFileDialog::getOpenFileName(this, tr("select a file"), "", tr("filetype(*.txt)"));
    if(filename.isEmpty()) // if not select any file, return
        return;
    QFile file(filename); // notice that filename is exactly the full path to the file
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //ui->CODE_text->setPlainText(file.readAll());
        while(!file.atEnd()) {
            QString command(file.readLine()); // transfer qbytearrary into qstring
            command = command.trimmed(); // delete the \n at the end of a line
            command_handler(command);
        }
    }
    file.close();
    show_code();
}

void MainWindow::on_RUN_clicked()
{
    // on click run button
    // parsing
    // show the grammar tree
    // evaluating
}

void MainWindow::on_CLEAR_clicked()
{
   // on click clear button
    basic_program.clear();
    pro_data.clear();
    ui->CODE_text->clear();
    ui->RESULT_text->clear();
    ui->GRAMMAR_text->clear();
    ui->COMMAND_text->clear();
}

void MainWindow::on_COMMAND_text_returnPressed()
{
   // press enter in command input box
    QString command = ui->COMMAND_text->text();
    if (get_value) { // get the input
        input_value = command;
        get_value = false;
    } else if (command == "QUIT") {
        exit(0);
    } else if (command == "HELP") {
        show_help();
    } else if (command == "LOAD") {
        on_LOAD_clicked();
    } else if (command == "RUN") {
        on_RUN_clicked();
    } else if (command == "CLEAR") {
        on_CLEAR_clicked();
    } else {// input is regarded as statement command
        command_handler(command);
    }
    ui->COMMAND_text->clear();
    show_code();
}