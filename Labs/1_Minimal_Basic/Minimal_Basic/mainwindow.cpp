#include "mainwindow.h"
#include "ui_mainwindow.h"
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

void MainWindow::constructExp(QString in_str) {
    QString reg_pattern = "(\\d+)\\s+(REM|LET|PRINT|INPUT|GOTO|IF|END).*";
    QRegExp exp(reg_pattern);
    exp.indexIn(in_str);
    QString operation = exp.cap(2);
    qDebug() << exp.cap(2);
    if (operation == "LET") {
        QRegExp let_pattern("\\d+\\s+LET\\s+(\\w+)\\s*=\\s*(.*)");
        let_pattern.indexIn(in_str);
        QString var = let_pattern.cap(1);
        QString exp = let_pattern.cap(2);
        data.insert(var, exp);
    } else if (operation == "PRINT") {
        QRegExp print_pattern("\\d+\\s+PRINT\\s+(\\w+)");
        print_pattern.indexIn(in_str);
        QString var = print_pattern.cap(1);
//        QString get_val(data.value(var));
        ui->RESULT_text->append(data.value(var));
    } else if (operation == "INPUT") {
        QRegExp input_pattern("\\d+\\s+INPUT\\s+(\\w+)");
        input_pattern.indexIn(in_str);
        QString var = input_pattern.cap(1);
    } else if (operation == "GOTO") {
        QRegExp goto_pattern("\\d+\\s+GOTO\\s+(\\d+)");
        goto_pattern.indexIn(in_str);
        int line_number = goto_pattern.cap(1).toInt();
    } else if (operation == "IF") {
        QRegExp if_pattern("\\d+\\s+IF\\s+(.*)?THEN\\s+(\\d+)");
        if_pattern.indexIn(in_str);
        QString if_exp = if_pattern.cap(1);
        int line_number = if_pattern.cap(2).toInt();
    } else if (operation == "END") {

    }
}


// show the code in the code_text box
void MainWindow::show_code() {
    ui->CODE_text->clear(); // clear the text first
    QMap<int, QString>::const_iterator tmp;
    tmp = basic_program.constBegin();
    while (tmp != basic_program.constEnd()) {
        ui->CODE_text->append(tmp.value());
        ++tmp;
    }
}

void MainWindow::show_help() {
    QMessageBox::information(this,
                             "HELP",
                             "USAGE: COMMAND or STATEMENT.\n"
                             "COMMANDs: LOAD, RUN, CLEAR, HELP, QUIT.\n"
                             "STATEMENT: Begin with a line number which should be a positve number.\n",
                             QMessageBox::Yes);
}

void MainWindow::on_LOAD_clicked()
{
   // on click load button
    QString filename = QFileDialog::getOpenFileName(this, tr("select a file"), "", tr("filetype(*.txt)"));
    if(filename.isEmpty()) // if not select any file, return
        return;
    // from this, notice that filename is exactly the full path to the file
    //qDebug() << filename;
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        //ui->CODE_text->setPlainText(file.readAll());
        while(!file.atEnd()) {
            QString command(file.readLine()); // transfer qbytearrary into qstring
            command = command.trimmed(); // delete the \n in the end of a line
            QStringList command_list = command.split(" "); // split the statement by " "
            // store the statement in the structure
            int line_number = command_list[0].toInt();
            if (line_number)
                basic_program.insert(line_number, command);
        }
    }
    file.close();
    show_code();
}

void MainWindow::on_RUN_clicked()
{
   // on click run button
    // show the grammar tree
    qDebug() << basic_program.keys();
    qDebug() << basic_program.values();
}

void MainWindow::on_CLEAR_clicked()
{
   // on click clear button
    basic_program.clear();
    ui->CODE_text->clear();
    ui->RESULT_text->clear();
    ui->GRAMMAR_text->clear();
    ui->COMMAND_text->clear();
}

void MainWindow::on_COMMAND_text_returnPressed()
{
   // press enter in command input box
    QString command = ui->COMMAND_text->text();
    constructExp(command);
    if (command == "QUIT") {
        exit(0);
    } else if (command == "HELP") {
        show_help();
    } else if (command == "LOAD") {
        on_LOAD_clicked();
    } else if (command == "RUN") {
        on_RUN_clicked();
    } else if (command == "CLEAR") {
        on_CLEAR_clicked();
    } else {// input is regarded as statement
        // split the statement according to " "
        QStringList command_list = command.split(" ");
        qDebug() << command_list;
        int line_number = command_list[0].toInt();
        // suppose that the line number is positive
        if (line_number) {
            // if the command is only a line number, then delete that line
            if (command_list.length() == 1)
                basic_program.remove(line_number);
            else {
                basic_program.insert(line_number, command); // store it by using line number as key
                ui->CODE_text->append(command);
            }
        } else
            show_help();
    }
    ui->COMMAND_text->clear();
    show_code();
}
