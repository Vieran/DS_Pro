#include "parser.h"
#include "exp.h"
#include <stack>
#include <string>

// get the next efficient operand or operator
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
// the last parameter is for determine where the exp_tree should be in the sta->exp_tree
// note: the last para is not elegant at all, but it should work
void construct_exp_tree(Statement *sta, QString purify_sta, int lmr) {
    QString value;
    int pos = 0;
    std::stack<QString> operator_stack;
    std::stack<Expression*> operand_stack;
    OP this_op, last_op = ENDOFSTR;  // initialize the last_op not as NUMBER or VALUE is necessary
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
                throw ("no matching right parenthesis");
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
            else if (this_op == SUB) {
                if (last_op != NUMBER && last_op != VALUE) {  // consider a negative number
                    this_op = tokenizer(purify_sta, value, pos);
                    if (this_op == NUMBER)
                        tmp =  new ConstantExp(-value.toUInt());
                    else if (this_op == VALUE)
                        tmp = new IdentifierExp("-"+ value);
                    else
                        throw ("error occurs when parsing!");
                    operand_stack.push(tmp);
                } else
                    operator_stack.push("-");
            }
            break;
        }
        case EQUAL:
            operator_stack.push("=");
            break;
        }
        last_op = this_op;
        qDebug() << value;
    }

    // finish all remaining operation
    while (!operator_stack.empty()) {
        top_op = operator_stack.top();
        operator_stack.pop();
        if (operand_stack.empty())
            throw ("error occurs when parsing!");
        rhs = operand_stack.top();
        operand_stack.pop();
        if (operand_stack.empty())
            throw ("error occurs when parsing!");
        lhs = operand_stack.top();
        operand_stack.pop();
        tmp = new CompoundExp(top_op, lhs, rhs);
        operand_stack.push(tmp);
    }

    switch (lmr) {
    case -1:
        sta->exp_tree->setLHS(operand_stack.top());
        break;
    case 0:
        sta->exp_tree = operand_stack.top();
        break;
    case 1:
        sta->exp_tree->setRHS(operand_stack.top());
        break;
    }

    operand_stack.pop();
}

// construct function for if_then statement
void construct_exp_tree_ifthen(Statement *sta, QString purify_sta) {
    QRegExp ifthen_pattern("(.*)\\s+(>|<|=)\\s+(.*)");
    ifthen_pattern.indexIn(purify_sta);
    sta->exp_tree = new CompoundExp(ifthen_pattern.cap(2));
    construct_exp_tree(sta, ifthen_pattern.cap(1), -1);
    construct_exp_tree(sta, ifthen_pattern.cap(3), 1);
}