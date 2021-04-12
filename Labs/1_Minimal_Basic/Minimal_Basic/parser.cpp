#include "parser.h"
#include "exp.h"
#include <stack>
#include <string>

static OP tokenizer(QString str, QString &identifier, int &pos) {
    identifier = "";
    int len = str.length();
    QChar tmp = str.at(pos);

    // blank
    while (pos < len && (tmp=str.at(pos)) == ' ')
        pos++;

    if (pos == len)
        return ENDOFSTR;

    // identifier
    if (tmp == '_' || tmp.isLetter()) {
        while (pos < len && ((tmp = str.at(pos)) == '_' || tmp.isLetterOrNumber())) {
            identifier += tmp;
            pos++;
        }
        return VALUE;
    }

    // number
    if (tmp.isNumber()){
        while (pos < len && (tmp = str.at(pos)).isNumber()) {
            identifier += tmp;
            pos++;
        }
        return NUMBER;
    }

    // operator
    if (tmp == '=') {
        pos++;
        return EQUAL;
    } else if (tmp == '+') {
        pos++;
        return ADD;
    } else if (tmp == '-') {
        pos++;
        return SUB;
    } else if (tmp == '*') {
        pos++;
        return MULTI;
    } else if (tmp == '/') {
        pos++;
        return DIVIDE;
    } else if (tmp == '(') {
        pos++;
        return LEFT_PARENTHESIS;
    } else if (tmp == ')') {
        pos++;
        return RIGHT_PARENTHESIS;
    }
}

// clarify the type of a given expression
ExpressionType type_of_exp(QString in_str) {
    QRegExp is_constant("^\\s*\\d+\\s*$");
    QRegExp is_identifier("^\\s*\\w+\\s*$");
    if (is_constant.indexIn(in_str) != -1) {
        //qDebug() << "constant " << in_str;
        return CONSTANT;
    }
    else if (is_identifier.indexIn(in_str) != -1) {
        //qDebug() << "identifier " << in_str;
        return IDENTIFIER;
    }
    else {
        //qDebug() << "compound " << in_str;
        return COMPOUND;
    }
}

// construct the expression tree
void construct_exp_tree(Statement *sta, QString purify_sta) {
    QString value;
    int pos = 0;
    std::stack<QString> operator_stack;
    std::stack<Expression*> operand_stack;
    OP this_op;
    QString top_op;
    Expression *tmp, *lhs, *rhs;
    int len = purify_sta.length();
    while (pos < len) {
        this_op = tokenizer(purify_sta, value, pos);
        switch (this_op) {
        case NUMBER:
            tmp = new ConstantExp(value.toInt());
            operand_stack.push(tmp);
            break;
        case VALUE:
            tmp = new IdentifierExp(value);
            operand_stack.push(tmp);
            break;
        case LEFT_PARENTHESIS:
            operator_stack.push("(");
            break;
        case RIGHT_PARENTHESIS: {
            while (!operator_stack.empty() && (top_op = operator_stack.top()) != "(") {
                operator_stack.pop();
                rhs = operand_stack.top();
                operand_stack.pop();
                lhs = operand_stack.top();
                operand_stack.pop();
                tmp = new CompoundExp(top_op, lhs, rhs);
                operand_stack.push(tmp);
            }
            if (top_op != "(")  // not matching
                error("no matching right parenthesis");
            else
                operator_stack.pop();  // pop the '('
            break;
        }
        case MULTI: case DIVIDE: {
            while (!operator_stack.empty() && ((top_op=operator_stack.top()) != "+" && top_op != "-" && top_op != "=" && top_op != "(")) {
                operator_stack.pop();
                rhs = operand_stack.top();
                operand_stack.pop();
                lhs = operand_stack.top();
                operand_stack.pop();
                tmp = new CompoundExp(top_op, lhs, rhs);
                operand_stack.push(tmp);
            }
            if (this_op == MULTI)
                operator_stack.push("*");
            else if (this_op ==DIVIDE)
                operator_stack.push("/");
            break;
        }
        case ADD: case SUB: {
            while (!operator_stack.empty() && ((top_op=operator_stack.top()) != "(" && top_op != "=")) {
                operator_stack.pop();
                rhs = operand_stack.top();
                operand_stack.pop();
                lhs = operand_stack.top();
                operand_stack.pop();
                tmp = new CompoundExp(top_op, lhs, rhs);
                operand_stack.push(tmp);
            }
            if (this_op == ADD)
                operator_stack.push("+");
            else if (this_op == SUB)
                operator_stack.push("-");
            break;
        }
        case EQUAL:
            operator_stack.push("=");
            break;
        }
        qDebug() << value;
    }

    // finish all remaining operation
    while (!operator_stack.empty()) {
        top_op = operator_stack.top();
        operator_stack.pop();
        rhs = operand_stack.top();
        operand_stack.pop();
        lhs = operand_stack.top();
        operand_stack.pop();
        tmp = new CompoundExp(top_op, lhs, rhs);
        operand_stack.push(tmp);
    }

    sta->exp_tree = operand_stack.top();
    operand_stack.pop();
}

// construct function for if_then statement
void construct_exp_tree_ifthen(Statement *sta, QString purify_sta) {
}