#include "program.h"
#include "parser.h"

// error taken func
void error(QString error_msg) {
    // handle the error
    qDebug() << error_msg;
}

void Statement::clear_expt(Expression *expt) {
    if (expt == nullptr)
        return;

    if (expt->type() == COMPOUND) {
        clear_expt(expt->getLHS());
        clear_expt(expt->getRHS());
    }
    delete expt;
}