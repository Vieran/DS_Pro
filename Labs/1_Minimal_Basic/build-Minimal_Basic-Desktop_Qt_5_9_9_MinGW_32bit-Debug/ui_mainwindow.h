/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.9
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *CODE;
    QLabel *RESULT;
    QLabel *GRAMMAR;
    QPlainTextEdit *CODE_text;
    QTextBrowser *GRAMMAR_text;
    QPushButton *LOAD;
    QPushButton *RUN;
    QPushButton *CLEAR;
    QLabel *COMMAND_input;
    QTextBrowser *RESULT_text;
    QLineEdit *COMMAND_text;
    QMenuBar *menubar;
    QMenu *menuGUI_Basic;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(793, 1013);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QStringLiteral("centralwidget"));
        CODE = new QLabel(centralwidget);
        CODE->setObjectName(QStringLiteral("CODE"));
        CODE->setGeometry(QRect(20, 10, 31, 16));
        RESULT = new QLabel(centralwidget);
        RESULT->setObjectName(QStringLiteral("RESULT"));
        RESULT->setGeometry(QRect(410, 10, 51, 16));
        GRAMMAR = new QLabel(centralwidget);
        GRAMMAR->setObjectName(QStringLiteral("GRAMMAR"));
        GRAMMAR->setGeometry(QRect(20, 450, 91, 16));
        CODE_text = new QPlainTextEdit(centralwidget);
        CODE_text->setObjectName(QStringLiteral("CODE_text"));
        CODE_text->setGeometry(QRect(20, 30, 360, 400));
        GRAMMAR_text = new QTextBrowser(centralwidget);
        GRAMMAR_text->setObjectName(QStringLiteral("GRAMMAR_text"));
        GRAMMAR_text->setGeometry(QRect(20, 470, 751, 380));
        LOAD = new QPushButton(centralwidget);
        LOAD->setObjectName(QStringLiteral("LOAD"));
        LOAD->setGeometry(QRect(20, 860, 210, 25));
        RUN = new QPushButton(centralwidget);
        RUN->setObjectName(QStringLiteral("RUN"));
        RUN->setGeometry(QRect(290, 860, 210, 25));
        CLEAR = new QPushButton(centralwidget);
        CLEAR->setObjectName(QStringLiteral("CLEAR"));
        CLEAR->setGeometry(QRect(560, 860, 210, 25));
        COMMAND_input = new QLabel(centralwidget);
        COMMAND_input->setObjectName(QStringLiteral("COMMAND_input"));
        COMMAND_input->setGeometry(QRect(20, 900, 101, 16));
        RESULT_text = new QTextBrowser(centralwidget);
        RESULT_text->setObjectName(QStringLiteral("RESULT_text"));
        RESULT_text->setGeometry(QRect(410, 30, 360, 400));
        COMMAND_text = new QLineEdit(centralwidget);
        COMMAND_text->setObjectName(QStringLiteral("COMMAND_text"));
        COMMAND_text->setGeometry(QRect(20, 920, 760, 30));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QStringLiteral("menubar"));
        menubar->setGeometry(QRect(0, 0, 793, 26));
        menuGUI_Basic = new QMenu(menubar);
        menuGUI_Basic->setObjectName(QStringLiteral("menuGUI_Basic"));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName(QStringLiteral("statusbar"));
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuGUI_Basic->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        CODE->setText(QApplication::translate("MainWindow", "CODE", Q_NULLPTR));
        RESULT->setText(QApplication::translate("MainWindow", "RESULT", Q_NULLPTR));
        GRAMMAR->setText(QApplication::translate("MainWindow", "GRAMMAR TREE", Q_NULLPTR));
        LOAD->setText(QApplication::translate("MainWindow", "LOAD", Q_NULLPTR));
        RUN->setText(QApplication::translate("MainWindow", "RUN", Q_NULLPTR));
        CLEAR->setText(QApplication::translate("MainWindow", "CLEAR", Q_NULLPTR));
        COMMAND_input->setText(QApplication::translate("MainWindow", "COMMAND INPUT", Q_NULLPTR));
        menuGUI_Basic->setTitle(QApplication::translate("MainWindow", "GUI Basic", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
