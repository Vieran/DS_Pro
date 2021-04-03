#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include "statement.h"
#include <QMap>

extern EvaluationContext pro_data;
extern QMap<int, Statement> basic_program;
extern bool get_value;
extern QString input_value;

void error(QString error_msg);
//void to_get_input(QString var);

#endif // PROGRAM_H