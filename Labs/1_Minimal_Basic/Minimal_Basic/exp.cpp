/* the implement of exp.h */
#include "exp.h"
#include "program.h"

/* Expression */
// these are just default function implement
Expression::Expression(){}
Expression::~Expression(){}
int Expression::getConstatntValue(){}
QString Expression::getIdentifierName(){}
QString Expression::getOperator(){}
Expression* Expression::getLHS(){}
Expression* Expression::getRHS(){}
void Expression::setLHS(Expression *lhs_t){}
void Expression::setRHS(Expression *rhs_t){}

/* ConstanExp */
int ConstantExp::eval(EvaluationContext &context) {
    return value;
}

QString ConstantExp::toString() {
    return QString::number(value);
}

ExpressionType ConstantExp::type() {
    return CONSTANT;
}

/* IdentifierExp */
int IdentifierExp::eval(EvaluationContext &context) {
    if(!context.isDefined(name))
        throw (name + " is undefined");
    return context.getValue(name);
}

QString IdentifierExp::toString() {
    return name;
}

ExpressionType IdentifierExp::type() {
    return IDENTIFIER;
}
QString IdentifierExp::getIdentifierName() {
    return name;
}

/* CompoundExp */
int CompoundExp::eval(EvaluationContext &context) {
    // handle string variable
    if (rhs->type() == STRING) {
        context.setString(lhs->toString(), rhs->toString());
        return 0;
    }

    int right = rhs->eval(context);
    if(op == "=") {
        context.setValue(lhs->getIdentifierName(), right);
        return right;
    }

    int left = lhs->eval(context);
    if(op == "+")
        return left + right;
    if(op == "-")
        return  left - right;
    if(op == "*")
        return left * right;
    if(op == "/") {
        if(right == 0)
            throw ("division by 0");
        return left / right;
    }
    throw ("illegal operator in expression");
    return 0;
}

// what does this func do?
QString CompoundExp::toString() {
    return getOperator();
}

ExpressionType CompoundExp::type() {
    return COMPOUND;
}

QString CompoundExp::getOperator() {
    return op;
}

Expression* CompoundExp::getLHS() {
    return lhs;
}


Expression* CompoundExp::getRHS() {
    return rhs;
}
void CompoundExp::setLHS(Expression *lhs_t) {
    lhs = lhs_t;
}
void CompoundExp::setRHS(Expression *rhs_t) {
    rhs = rhs_t;
}

/* StringExp */
int StringExp::eval(EvaluationContext &context) {
    return 0;
}

QString StringExp::toString(){
    return str;
}

ExpressionType StringExp::type() {
    return STRING;
}

/* EvaluationContext */
void EvaluationContext::setValue(QString var, int value) {
    symbolTable.insert(var, value);
}

void EvaluationContext::setString(QString var, QString value) {
    stringTable.insert(var, value);
}

int EvaluationContext::getValue(QString var) {
    return symbolTable.value(var);
}

QString EvaluationContext::getStr(QString var) {
    return stringTable.value(var);
}

bool EvaluationContext::isDefined(QString var) {
    return symbolTable.contains(var);
}

bool EvaluationContext::str_isDefined(QString var) {
    return stringTable.contains(var);
}

// delete all the elements
void EvaluationContext::clear() {
    symbolTable.clear();
}