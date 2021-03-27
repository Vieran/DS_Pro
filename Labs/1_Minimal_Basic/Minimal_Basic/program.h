#ifndef PROGRAM_H
#define PROGRAM_H

/* store the data during the execution */
#include "exp.h"
#include "statement.h"
#include <QMap>

extern EvaluationContext pro_data;
extern QMap<int, QString> basic_program;

void error(QString error_msg);

#endif // PROGRAM_H