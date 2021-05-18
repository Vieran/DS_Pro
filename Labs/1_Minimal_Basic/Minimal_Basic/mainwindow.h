#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include <QQueue>
#include "exp.h"
#include "program.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_LOAD_clicked();

    void on_RUN_clicked();

    void on_CLEAR_clicked();

    void on_COMMAND_text_returnPressed();

    void command_handler(QString in_str);

    void on_DEBUG_STEP_clicked();

private:
    Ui::MainWindow *ui;
    QMap<int, Statement> basic_program;  // hold all statements by line number
    EvaluationContext variable;
    QQueue<var_in> var_to_input;
    int next_line;  // pc for the execution
    bool debug_mode;

    // tool func
    void show_code();
    void display_exp(QString indent, Expression *expt);
    void show_grammartree();
    void show_help();

    // handling error
    bool error_occur;
    void error_handler(QString error_msg);

    // display all kind of statements
    void rem_dis(Statement *sta);
    void end_dis(Statement *sta);
    void let_dis(Statement *sta);
    void print_dis(Statement *sta);
    void printf_dis(Statement *sta);
    void input_dis(Statement *sta);
    void inputs_dis(Statement *sta);
    void ifthen_dis(Statement *sta);
    void goto_dis(Statement *sta);
    void single_cmd_display(Statement *sta);

    // functions for constructing some kind of statements
    void let_handler(Statement *sta);
    void print_handler(Statement *sta);
    void printf_handler(Statement *sta);
    void ifthen_handler(Statement *sta);

    // execute
    void execute();
    void let_exe(Statement *sta);
    void print_exe(Statement *sta);
    void printf_exe(Statement *sta);  // todo
    void input_exe(Statement *sta);
    void inputs_exe(Statement *sta);
    int ifthen_exe(Statement *sta);
    int goto_exe(Statement *sta);
    int single_cmd_exe(Statement *sta);

    // add new functions
    void highlight();
    void show_variable();

    //private static MainWindow instance = new MainWindow();
    // singleton pattern or cpp static member
    // qeventloop
};
#endif // MAINWINDOW_H