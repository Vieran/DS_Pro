#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMap>

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

private:
    Ui::MainWindow *ui;
    QMap<int, QString> basic_program;
    QMap<QString, QString> data;

    void show_code();
    void show_help();
    void constructExp(QString in_str);
};
#endif // MAINWINDOW_H
