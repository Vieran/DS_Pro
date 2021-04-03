/* the implement of exp.h */
#include "exp.h"
#include "program.h"

/* Expression */
// these are just default function implement
Expression::~Expression(){}
int Expression::getConstatntValue(){}
QString Expression::getIdentifierName(){}
QString Expression::getOperator(){}
Expression* Expression::getLHS(){}
Expression* Expression::getRHS(){}

/* ConstanExp */
QString ConstantExp::toString() {
    return QString(value);
}

ExpressionType ConstantExp::type() {
    return CONSTANT;
}

int ConstantExp::getConstatntValue() {
    return value;
}

/* IdentifierExp */
int IdentifierExp::eval(EvaluationContext &context) {
    if(!context.isDefined(name))
        error(name + " is undefined");
    return context.getValue(name);
}

QString IdentifierExp::toString() {
    if(!pro_data.isDefined(name))
        error(name + " is not defined");
    return QString(pro_data.getValue(name));
}

ExpressionType IdentifierExp::type() {
    return IDENTIFIER;
}

QString IdentifierExp::getIdentifierName() {
    return name;
}

/* CompoundExp */
int CompoundExp::eval(EvaluationContext &context) {
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
            error("division by 0");
        return left / right;
    }
    error("illegal operator in expression");
    return 0;
}

// what does this func do?
QString CompoundExp::toString() {}

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

/* EvaluationContext */
void EvaluationContext::setValue(QString var, int value) {
    symbolTable.insert(var, value);
}

int EvaluationContext::getValue(QString var) {
    return symbolTable.value(var);
}

bool EvaluationContext::isDefined(QString var) {
    return symbolTable.contains(var);
}

// delete all the elements
void EvaluationContext::clear() {
    symbolTable.clear();
}