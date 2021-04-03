#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>
#include "exp.h"
#include "statement.h"

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
    EvaluationContext pro_data;
    QMap<int, Statement> basic_program;
    void show_code();
    void show_help();

    //private static MainWindow instance = new MainWindow();
    // singleton pattern
    // cpp static member
    // all in mainwindow
};
#endif // MAINWINDOW_H