#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "program.h"
#include "parser.h"
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    error_occur = false;
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
        ui->CODE_text->append(tmp.value().statement);
        ++tmp;
    }
}

// display a specific expression tree in the grammar_text box
void MainWindow::display_exp(QString indent, Expression *expt) {
    if (expt == nullptr)
        return;

    const QString tab = "    ";
    indent += tab;
    ui->GRAMMAR_text->append(indent + expt->toString());

    if (expt->type() == COMPOUND) {
        display_exp(indent, expt->getLHS());
        display_exp(indent, expt->getRHS());
    }
}

// show the grammar tree in the grammar_text box
void MainWindow::show_grammartree() {
    ui->GRAMMAR_text->clear();  // clear the text first
    QMap<int, Statement>::iterator tmp;
    tmp = basic_program.begin();
    Statement *current_sta;
    while (tmp != basic_program.end()) {
        current_sta = &(tmp.value());
        switch (current_sta->sta_type) {
        case REM:
            rem_dis(current_sta);
            break;
        case END:
            end_dis(current_sta);
            break;
        case LET:
            let_dis(current_sta);
            break;
        case PRINT:
            print_dis(current_sta);
            break;
        case INPUT:
            input_dis(current_sta);
            break;
        case IF_THEN:
            ifthen_dis(current_sta);
            break;
        case GOTO:
            goto_dis(current_sta);
            break;
        }
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

// handler error
void MainWindow::error_handler(QString error_msg) {
    QMessageBox::information(this,
                             "ERROR",
                             error_msg,
                             QMessageBox::Ok);
    error_occur = true;
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
        else {
            QRegExp reg_exp_("\\s*(\\d+)\\s+(\\w+)(.*)");
            reg_exp_.indexIn(in_str);
            QString tmp = reg_exp_.cap(2);
            if (tmp != "REM" && tmp != "LET" && tmp != "PRINT" && tmp != "INPUT" && tmp != "GOTO" && tmp != "IF" && tmp != "END") {
                error_handler("illegal statement");
                return;
            }
            basic_program.insert(line_number, in_str);
        }
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
    error_occur = false;
    // construct the expresstion tree
    QMap<int, Statement>::iterator it = basic_program.begin();
    while (it != basic_program.end()) {
        //qDebug() << it.value().statement;
        if (it.value().sta_type == LET)
            let_handler(&(it.value()));
        else if (it.value().sta_type == PRINT)
            print_handler(&(it.value()));
        else if (it.value().sta_type == INPUT)
            input_handler(&(it.value()));
        else if (it.value().sta_type == IF_THEN)
            ifthen_handler(&(it.value()));
        it++;
    }
    show_grammartree();

    if (error_occur)
        return;

    if (!var_to_input.empty())
        ui->COMMAND_input->setText("input: " + var_to_input.head());
    else
        execute();  // no variable to input, execute immediately
}

void MainWindow::on_CLEAR_clicked()
{
   // on click clear button
    basic_program.clear();
    variable.clear();
    ui->CODE_text->clear();
    ui->RESULT_text->clear();
    ui->GRAMMAR_text->clear();
    ui->COMMAND_text->clear();
}

void MainWindow::on_COMMAND_text_returnPressed()
{
   // press enter in command input box
    QString command = ui->COMMAND_text->text();
    if(!var_to_input.empty()){
        variable.setValue(var_to_input.dequeue(), command.toUInt());
        if (!var_to_input.empty())
            ui->COMMAND_input->setText("input: " + var_to_input.head());
        else {
            execute();  // get all the variable and execute
            ui->COMMAND_input->setText("GRAMMAR TREE");  // reset the input title
        }
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
    } else {  // input is regarded as statement command
        command_handler(command);
    }
    ui->COMMAND_text->clear();
    show_code();
}