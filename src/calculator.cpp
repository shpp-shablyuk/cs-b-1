#include <iostream>
#include "console.h"
#include <string>
#include <stack>
#include <cmath>
#include <map>
#include <vector>
#include <exception>

#include "customStack.h"

using namespace std;

class Calculator {
    string expression;
    double result = 0;
    public:
        Calculator(string exp = ""): expression(exp) {
            if (!expression.empty()) {
                calculate();
            }
        }

        void showResult(void) {

            cout << "Result: " << result << endl;
        }
        double getResult() {
            return result;
        }

    private:
        struct Token {
            string type;
            string value;
            int priority = -5;
        };

        map<string, int> operators = {
            {"-", 2},
            {"+", 2},
            {"-x", 2},
            {"/", 3},
            {"*", 3},
            {"^", 4},
            {"(", 0},
            {")", 0},
        };

        /**
         * @brief calculate expression
         */
        void calculate() {
            if (expression.empty()) {
                return;
            }

            vector<Token> tokens = getTokens(expression);

//            printTokens(tokens);
//            return;

            CustomStack<double> digitStack;
            CustomStack<Token> operatorsStack;

            for (vector<Token>::iterator it = tokens.begin(); it != tokens.end(); it++) {
                Token token = *it;

                if (token.type == "digit") {             // if digit
                    digitStack.push(stod(token.value));
                } else if (token.value == "(") {            // if "("
                    operatorsStack.push(token);
                } else if (token.value == ")") {
                    //  to calculating the expression in parentheses
                    while (operatorsStack.top().value != "(") {
                        if (digitStack.empty() || operatorsStack.empty()) {
                            throw "incorrect expression";
                        }
                        perfomAction(digitStack, operatorsStack);
                    }
                    if (operatorsStack.top().value == "(") {
                        operatorsStack.pop();
                    }
                } else if(token.type == "operator") {    // if operator
                    //  to calculating the expression until operator in stack higer priority than current operator
                    while (!operatorsStack.empty() && isLessPriority(token.priority, operatorsStack.top().priority)) {
                            perfomAction(digitStack, operatorsStack);
                    }

                    operatorsStack.push(token);
                }

//                printDigit(digitStack);
//                printOperators(operatorsStack);
            }

            while(!operatorsStack.empty()) {
                perfomAction(digitStack, operatorsStack);
            }


            result = digitStack.top();
        }

        /**
         * @brief partition string into tokens
         * @param expression
         * @return
         */
        vector<Token> getTokens(string expression) {
            vector<Token> tokens;
            int expressionLength = expression.length();
            string token = "";

            for (int i = 0; i < expressionLength; i++) {
                char currentChar = expression[i];

                if (isspace(currentChar)) {
                    continue;
                }

                if (isCharDigit(currentChar)) {             // if digit
                    token += currentChar;
                    //  combining adjacent digits in the number
                    if ((i != expressionLength - 1) && isCharDigit(expression[i + 1])) {
                        continue;
                    }

                    Token sToken;
                    sToken.type = "digit";
                    sToken.value = token;
                    tokens.push_back(sToken);
                } else if(currentChar == '-' && (i == 0 || expression[i - 1] == '(')) {    // if -x
                    token += currentChar;

                    Token sToken;
                    sToken.type = "operator";
                    sToken.value = "-x";
                    sToken.priority = operators[token];
                    tokens.push_back(sToken);
                } else if(isCharOperator(currentChar)) {    // if operator
                    token += currentChar;

                    Token sToken;
                    sToken.type = "operator";
                    sToken.value = token;
                    sToken.priority = operators[token];
                    tokens.push_back(sToken);
                } else {
                    throw "undefined token [" + string(&currentChar) + "]";
                }

                token = "";
            }

            return tokens;
        }

        void perfomAction(CustomStack<double>& digitStack, CustomStack<Token>& operatorsStack){
            double res = calculateVars(digitStack, operatorsStack.top().value);
            digitStack.push(res);

            operatorsStack.pop();
        }

        bool isCharOperator(char c) {
            string opr;
            opr += c;
            return operators.count(opr) != 0;
        }

        bool isCharDigit(char c) {
            return (isdigit(c) || c == '.') ? true : false;
        }

        double calculateVars(CustomStack<double>& digitStack, string opr) {

            if (opr == "+") {
                if (digitStack.size() < 2) {
                    throw "incorrect expression";
                }
                double var2 = getVarible(digitStack);
                double var1 = getVarible(digitStack);

                return var1 + var2;
            } else if (opr == "-") {
                if (digitStack.size() < 2) {
                    throw "incorrect expression";
                }
                double var2 = getVarible(digitStack);
                double var1 = getVarible(digitStack);

                return var1 - var2;
            } else if (opr == "*") {
                if (digitStack.size() < 2) {
                    throw "incorrect expression";
                }
                double var2 = getVarible(digitStack);
                double var1 = getVarible(digitStack);

                return var1 * var2;
            } else if (opr == "/") {
                if (digitStack.size() < 2) {
                    throw "incorrect expression";
                }
                double var2 = getVarible(digitStack);
                double var1 = getVarible(digitStack);

                return var1 / var2;
            } else if (opr == "^") {
                if (digitStack.size() < 2) {
                    throw "incorrect expression";
                }
                double var2 = getVarible(digitStack);
                double var1 = getVarible(digitStack);

                return pow(var1, var2);
            } else if (opr == "-x") {
                if (digitStack.empty()) {
                    throw "incorrect expression";
                }
                double var1 = getVarible(digitStack);

                return var1 * (-1);
            } else {
                throw "unknown [" + opr + "] operator!";
            }
        }

        double getVarible(CustomStack<double>& digitStack) {
            double var = digitStack.top();
            digitStack.pop();

            return var;
        }

        bool isLessPriority(int token, int tokenInStack) {
            return (token <=  tokenInStack);
        }

        //  for debuging
        void printTokens(vector<Token>& tokens) {
            for (vector<Token>::iterator it = tokens.begin(); it != tokens.end(); it++) {
                pintToken(*it);
            }
        }

        //  for debuging
        void pintToken(Token token) {
            cout << "[" << token.value << "]";
            cout << "[" << token.type << "]";
            cout << "[" << token.priority << "]" << endl;
        }

        //  for debuging
        void printOperators(CustomStack<Token> st) {
            if (st.empty()) {
                std::cout << "empty" << endl;
                return;
            }

            while (!st.empty())
             {
                pintToken(st.top());
                st.pop();
             }
            std::cout << endl;
        }

        //  for debuging
        void printDigit(CustomStack<double> st) {
            if (st.empty()) {
                std::cout << "empty" << endl;
                return;
            }

            while (!st.empty())
             {
                cout << "[" << st.top() << "]";
                st.pop();
             }
            std::cout << endl;
        }
};

struct testExpression {
    string expr;
    double result;
};

void test() {
    testExpression test1, test2, test3, test4;
    test1.expr = "(3 + 4) * (5 - 8) /2 * 2";
    test1.result = -21;

    test2.expr = "33.67 + 22.33 - 41.1 / 8.22 * 2 ";
    test2.result = 46;

    test3.expr = "12 * 5 + (-5) / 10";
    test3.result = 59.5;

    test4.expr = "2 * 2 ^ 2 / 8 + 5";
    test4.result = 6;

    testExpression exp[] {test1, test2, test3, test4};

    for (int i = 0; i < 4; i++) {
        testExpression curExp = exp[i];
        Calculator calc(curExp.expr);
        double curRes =  calc.getResult();

        string status = (curRes == curExp.result) ? "PASS" : "FAIL";

        cout << status << " ";
        cout << curExp.expr << " = " << curExp.result << " [" << curRes << "]" << endl;
    }
}


int main() {
    //  for testing
    test();

    while (true) {
        cout << "Enter expression: ";
        string expression;
        getline(cin, expression);

        Calculator calc(expression);
        calc.showResult();
    }

    return 0;
}
