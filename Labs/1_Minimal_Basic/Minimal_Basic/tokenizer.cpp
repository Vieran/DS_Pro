/* transfer an input into a statement
 * STL
 */

#include <QString>
#include <QRegExp>
#include <QtDebug>

enum statement{REM, LET, PRINT, INPUT, GOTO, IF, END};
enum OP{equal, greater, less, add, sub, multi, divide, left_parenthesis, right_parenthesis}; // =><+-*/()
class expression {
private:
    struct CompoundExp;
    typedef union exp {
        int constant_exp;
        OP identifier_exp;
        CompoundExp *compound_exp; // forward declaration can just be used as pointer or reference
    } exp;
    struct CompoundExp {
        exp left_exp;
        OP operation;
        exp right_exp;
    }; // what's difference with or without typedef?
};
