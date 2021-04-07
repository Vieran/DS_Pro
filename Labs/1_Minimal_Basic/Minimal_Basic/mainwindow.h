#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
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

private:
    Ui::MainWindow *ui;
    QMap<int, Statement> basic_program;  // hold all statements by line number
    QMap<QString, int> variable;  // hold all variables by name
    void show_code();
    void show_help();

    // handler for all kinds of statements
    void rem_handler(Statement sta);
    void let_handler(Statement sta);
    void print_handler(Statement sta);
    void input_handler(Statement sta);
    void goto_handler(Statement sta);
    void ifthen_handler(Statement sta);
    void end_handler(Statement sta);

    //private static MainWindow instance = new MainWindow();
    // singleton pattern or cpp static member
};
#endif // MAINWINDOW_H