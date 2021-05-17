/* File: exp.h
 * defines a class hierarchy for arithmetic expression
 */

#ifndef EXP_H
#define EXP_H

#include <QString>
#include <QMap>
//#include "tokenscanner.h"

/* forward reference */
class EvaluationContext;

/* type of the expression */
enum ExpressionType{CONSTANT, IDENTIFIER, COMPOUND, STRING};

/* Class: Expression
 * represent a node in an expression tree
 * is an abstract class
 */
class Expression {
public:
    Expression();
    virtual ~Expression();
    virtual int eval(EvaluationContext &context) = 0;
    virtual QString toString() = 0;
    virtual ExpressionType type() = 0;

    // these are not neccessary in every subclass
    virtual int getConstatntValue();
    virtual QString getIdentifierName();
    virtual QString getOperator();
    virtual Expression* getLHS();
    virtual Expression* getRHS();
    virtual void setLHS(Expression *lhs_t);
    virtual void setRHS(Expression *rhs_t);
};

/* Class: ConstanExp */
class ConstantExp : public Expression {
public:
    ConstantExp(int val):value(val) {}
    virtual ~ConstantExp() {}

    virtual int eval(EvaluationContext &context);
    virtual QString toString();
    virtual ExpressionType type();

private:
    int value;
};

/* Class: IdentifierExp */
class IdentifierExp : public Expression {
public:
    IdentifierExp(QString name):name(name) {}
    virtual ~IdentifierExp(){}

    virtual int eval(EvaluationContext &context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getIdentifierName();

private:
    QString name;
};

/* Class: CompoundExp */
class CompoundExp : public Expression {
public:
    CompoundExp(QString op, Expression *lhs=nullptr, Expression *rhs=nullptr):op(op), lhs(lhs), rhs(rhs){}
    virtual ~CompoundExp(){}

    virtual int eval(EvaluationContext &context);
    virtual QString toString();
    virtual ExpressionType type();

    virtual QString getOperator();
    virtual Expression *getLHS();
    virtual Expression *getRHS();
    virtual void setLHS(Expression *lhs_t);
    virtual void setRHS(Expression *rhs_t);

private:
    QString op;
    Expression *lhs, *rhs;
};

class StringExp : public Expression {
public:
    StringExp(QString str):str(str) {}
    virtual int eval(EvaluationContext &context);
    virtual QString toString();
    virtual ExpressionType type();

private:
    QString str;
};

/* Class: EvaluationContext
 * encapsulate the information that the evaluator needs to know in order to evaluate an expresssion
 */
class EvaluationContext {
public:
    void setValue(QString var, int value);
    void setString(QString var, QString value);
    int getValue(QString var);
    QString getStr(QString var);
    bool isDefined(QString var);
    bool str_isDefined(QString var);
    void clear();

private:
    QMap<QString, int> symbolTable;
    QMap<QString, QString> stringTable;  // store string variable
};

#endif // EXP_H