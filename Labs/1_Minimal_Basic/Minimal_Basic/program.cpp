#include "program.h"
#include "parser.h"
Statement::Statement(QString in_str) {
    statement = in_str;
    QRegExp reg_exp("\\s*(\\d+)\\s+(\\w+)(.*)");
    reg_exp.indexIn(in_str);
    QString sta_t = reg_exp.cap(2);
    if (sta_t == "REM")
        sta_type = REM;
    else if (sta_t == "LET")
        sta_type = LET;
    else if (sta_t == "PRINT")
        sta_type = PRINT;
    else if (sta_t == "PRINTF")
        sta_type = PRINTF;
    else if (sta_t == "INPUT")
        sta_type = INPUT;
    else if (sta_t == "INPUTS")
        sta_type = INPUTS;
    else if (sta_t == "GOTO")
        sta_type = GOTO;
    else if (sta_t == "IF")
        sta_type = IF_THEN;
    else if (sta_t == "END")
        sta_type = END;
    else
        throw ("error when constructing!");

    exp_tree = nullptr;
}

// tool function for destructor of Statement
void Statement::clear_expt(Expression *expt) {
    if (expt == nullptr)
        return;

    if (expt->type() == COMPOUND) {
        clear_expt(expt->getLHS());
        clear_expt(expt->getRHS());
    }
    delete expt;
}