#include "program.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QRegExp>
#include <QDebug>

// traverse and execute all statement
void MainWindow::execute() {
    QMap<int, Statement>::iterator tmp = basic_program.find(next_line);
    int tmp_line;
    while (tmp != basic_program.end() && !error_occur) {
        show_variable();
        Statement *current_sta = &(tmp.value());
        next_line = (++tmp).key();
        switch (current_sta->sta_type) {
        case REM:
            break;
        case END:
            return;
        case INPUT:
            input_exe(current_sta);
            break;
        case INPUTS:
            inputs_exe(current_sta);
            break;
        case LET:
            let_exe(current_sta);
            break;
        case PRINT:
            print_exe(current_sta);
            break;
        case PRINTF:
            printf_exe(current_sta);
            break;
        case IF_THEN:
            tmp_line = ifthen_exe(current_sta);
            if (tmp_line != -1)
                next_line = tmp_line;
            break;
        case GOTO:
            tmp_line = goto_exe(current_sta);
            if (tmp_line != -1)
                next_line = tmp_line;
            break;
        }
        if (!var_to_input.empty()) {
            ui->COMMAND_input->setText("input: " + var_to_input.head().var);
            return;
        }
        tmp = basic_program.find(next_line);
    }
}

// execute let statement
void MainWindow::let_exe(Statement *sta) {
    try {
        sta->exp_tree->eval(variable);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// execute print statememt
void MainWindow::print_exe(Statement *sta) {
    try {
        int result = sta->exp_tree->eval(variable);
        ui->RESULT_text->append(QString::number(result));
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
}

// fetch the var from the context
static inline QString get_var(QString var, EvaluationContext &context) {
    var = var.trimmed();
    int len = var.length();
    QString output = "";
    if ((var.at(0) == "\"" && var.at(len - 1) == "\"") || (var.at(0) == "\'" && var.at(len - 1) == "\'"))
        return var.mid(1, len - 2);
    else if (var.at(0).isNumber()) {
        int pos = 0;
        while (pos < len) {
            if (!var.at(pos).isNumber()) {
                throw ("error when dealing with the printf statement!");
                return "";
            }
            pos++;
        }
        return var;
    } else if (context.isDefined(var))
        return QString::number(context.getValue(var));
    else if (context.str_isDefined(var))
        return context.getStr(var);
    else
        throw (var + " not defined!");
}

// get the output pattern
static inline QString get_str(QString str, int &pos) {
    QChar tmp = str.at(pos);

    // remove the blank
    while ((tmp = str.at(pos)) == " ")
        pos++;

    if (tmp != "\"" && tmp != "'") {
        throw ("no comma found!");
        return "";
    }

    QString comma = tmp;  // record the first comma
    pos++;
    QString output = "";
    int len = str.length();
    while (pos < len && (tmp = str.at(pos)) != comma) {
        output += tmp;
        pos++;
    }

    // move pos to the first ","
    while (pos < len && (tmp = str.at(pos)) != ",")
        pos++;
    return output;
}

// execute printf statement
void MainWindow::printf_exe(Statement *sta) {
    QRegExp printf_pattern("\\s*\\d+\\s+PRINTF\\s+\(.*)");
    printf_pattern.indexIn(sta->statement);
    QString origin_str = printf_pattern.cap(1);
    int pos = 0;
    QString str = get_str(origin_str, pos);  // the string pattern
    QStringList var_list = origin_str.mid(pos+1, -1).split(",");  // variable list
    QString output = "";  // string to show on the box
    QString var;  // the variable to fill in
    int str_pos = 0, var_pos = 0;
    int str_len = str.length();
    while (str_pos < str_len) {
        QChar tmp = str.at(str_pos);
        if (tmp != "{") {
            output += tmp;
            str_pos++;
            continue;
        } else {
            if (var_pos > var_list.size()) {
                throw ("no enougth var to fill in!");
                return;
            }
            var = var_list[var_pos];
            var_pos++;
            try {
                output += get_var(var, variable);
            } catch (QString error_msg) {
                error_handler(error_msg);
                return;
            }
            str_pos += 2;  // {} take two palces
        }
    }
    ui->RESULT_text->append(output);
}

// execute input statement
void MainWindow::input_exe(Statement *sta) {
    QRegExp input_pattern("\\s*\\d+\\s+INPUT\\s+(\\w+)");
    input_pattern.indexIn(sta->statement);
    QString input_var;
    input_var = input_pattern.cap(1);
    var_in to_input(input_var, INTEGER_VAR);
    var_to_input.push_back(to_input);
}

void MainWindow::inputs_exe(Statement *sta) {
    QRegExp input_pattern("\\s*\\d+\\s+INPUTS\\s+(\\w+)");
    input_pattern.indexIn(sta->statement);
    QString input_string;
    input_string = input_pattern.cap(1);
    var_in to_input(input_string, STRING_VAR);
    var_to_input.push_back(to_input);
}

// execute if then statement
// return the line number when true
int MainWindow::ifthen_exe(Statement *sta) {
    Expression *lhs = sta->exp_tree->getLHS();
    Expression *rhs = sta->exp_tree->getRHS();
    int left_result;
    int right_result;
    try {
        left_result = lhs->eval(variable);
        right_result = rhs->eval(variable);
    } catch (QString error_msg) {
        error_handler(error_msg);
    }
    QString op = sta->exp_tree->getOperator();

    if ((op == "=" && left_result == right_result) || (op == ">" && left_result > right_result) || (op == "<" && left_result < right_result)) {
        QRegExp if_pattern("\\s*(\\d+)\\s+IF\\s+(.*)?\\s+THEN\\s+(\\d+)");
        if_pattern.indexIn(sta->statement);
        return if_pattern.cap(3).toUInt();
    }
    return -1;
}

// execute goto statement
// return the line number
int MainWindow::goto_exe(Statement *sta) {
    QRegExp goto_pattern("\\s*\\d+\\s+GOTO\\s+(\\d+)");
    goto_pattern.indexIn(sta->statement);
    return goto_pattern.cap(1).toUInt();
}

int MainWindow::single_cmd_exe(Statement *sta) {
    int tmp_line = -1;
    switch (sta->sta_type) {
    case REM:
        break;
    case END:
        ui->LOAD->setEnabled(true);
        ui->CLEAR->setEnabled(true);
        debug_mode = false;
        error_handler("debug end normally!");
        break;
    case INPUT:
        input_exe(sta);
        break;
    case INPUTS:
        inputs_exe(sta);
        break;
    case PRINT:
        print_exe(sta);
        break;
    case PRINTF:
        printf_exe(sta);
        break;
    case GOTO:
        tmp_line = goto_exe(sta);
        break;
    case LET:
        let_exe(sta);
        break;
    case IF_THEN:
        tmp_line = ifthen_exe(sta);
        break;
    }
    return tmp_line;
}