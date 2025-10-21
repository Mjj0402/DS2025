#include <iostream>
#include <string>
#include <cmath>
#include <cctype>
#include <stdexcept>

using namespace std;

// 栈模板类实现
template <typename T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;
        Node(T val) : data(val), next(nullptr) {}
    };
    
    Node* top;
    int size;

public:
    // 构造函数
    Stack() : top(nullptr), size(0) {}
    
    // 析构函数
    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
    
    // 入栈操作
    void push(T val) {
        Node* newNode = new Node(val);
        newNode->next = top;
        top = newNode;
        size++;
    }
    
    // 出栈操作
    T pop() {
        if (isEmpty()) {
            throw runtime_error("栈为空，无法执行出栈操作");
        }
        
        Node* temp = top;
        T val = temp->data;
        top = top->next;
        delete temp;
        size--;
        return val;
    }
    
    // 获取栈顶元素
    T getTop() const {
        if (isEmpty()) {
            throw runtime_error("栈为空，无法获取栈顶元素");
        }
        return top->data;
    }
    
    // 判断栈是否为空
    bool isEmpty() const {
        return top == nullptr;
    }
    
    // 获取栈的大小
    int getSize() const {
        return size;
    }
};

// 计算两个数的运算结果
double calculate(double num1, double num2, char op) {
    switch (op) {
        case '+': return num1 + num2;
        case '-': return num1 - num2;
        case '*': return num1 * num2;
        case '/': 
            if (num2 == 0) {
                throw runtime_error("除数不能为零");
            }
            return num1 / num2;
        case '^': return pow(num1, num2); // 幂运算
        default: throw runtime_error("未知运算符");
    }
}

// 计算单目函数运算结果
double calculateFunc(double num, const string& func) {
    if (func == "sin") return sin(num);
    if (func == "cos") return cos(num);
    if (func == "tan") return tan(num);
    if (func == "log") return log(num);   // 自然对数
    if (func == "log10") return log10(num); // 常用对数
    if (func == "sqrt") return sqrt(num);
    throw runtime_error("未知函数: " + func);
}

// 判断运算符优先级
int getPriority(char op) {
    switch (op) {
        case '(': return 0;
        case '+': 
        case '-': return 1;
        case '*': 
        case '/': return 2;
        case '^': return 3; // 幂运算优先级高于乘除
        default: return -1;
    }
}

// 检查表达式是否有效
bool isValidExpression(const string& expr) {
    // 检查括号是否匹配
    Stack<char> bracketStack;
    for (char c : expr) {
        if (c == '(') {
            bracketStack.push(c);
        } else if (c == ')') {
            if (bracketStack.isEmpty()) {
                return false; // 右括号没有对应的左括号
            }
            bracketStack.pop();
        }
    }
    if (!bracketStack.isEmpty()) {
        return false; // 左括号没有对应的右括号
    }
    
    // 检查运算符是否合法
    for (int i = 0; i < expr.size(); i++) {
        char c = expr[i];
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '^') {
            // 检查运算符前后是否合法
            if (i == 0 || i == expr.size() - 1) {
                return false; // 运算符不能在表达式开头或结尾
            }
            char prev = expr[i-1];
            char next = expr[i+1];
            if ((prev < '0' || prev > '9') && prev != ')' && prev != '.' 
                && !(prev >= 'a' && prev <= 'z')) {
                return false; // 运算符前不是数字、右括号、小数点或字母(函数)
            }
            if ((next < '0' || next > '9') && next != '(' && next != '.'
                && !(next >= 'a' && next <= 'z')) {
                return false; // 运算符后不是数字、左括号、小数点或字母(函数)
            }
        }
    }
    
    return true;
}

// 计算表达式的值
double evaluateExpression(const string& expr) {
    // 首先检查表达式是否有效
    if (!isValidExpression(expr)) {
        throw runtime_error("表达式无效");
    }
    
    Stack<double> numStack; // 存储操作数的栈
    Stack<char> opStack;    // 存储运算符的栈
    
    int i = 0;
    int n = expr.size();
    
    while (i < n) {
        // 跳过空格
        if (expr[i] == ' ') {
            i++;
            continue;
        }
        
        // 如果是字母，可能是函数名
        if (isalpha(expr[i])) {
            string func;
            while (i < n && isalpha(expr[i])) {
                func += expr[i];
                i++;
            }
            
            // 函数后面应该跟左括号
            if (i >= n || expr[i] != '(') {
                throw runtime_error("函数后缺少左括号");
            }
            i++; // 跳过左括号
            
            // 计算括号内的表达式
            // 找到匹配的右括号
            int j = i;
            int bracketCount = 1;
            while (j < n && bracketCount > 0) {
                if (expr[j] == '(') bracketCount++;
                else if (expr[j] == ')') bracketCount--;
                j++;
            }
            
            if (bracketCount > 0) {
                throw runtime_error("函数括号不匹配");
            }
            
            string subExpr = expr.substr(i, j - i - 1);
            double subResult = evaluateExpression(subExpr);
            numStack.push(calculateFunc(subResult, func));
            
            i = j; // 移动到右括号之后
        }
        // 如果是数字或小数点，解析完整的数字
        else if (isdigit(expr[i]) || expr[i] == '.') {
            double num = 0;
            // 解析整数部分
            while (i < n && isdigit(expr[i])) {
                num = num * 10 + (expr[i] - '0');
                i++;
            }
            // 解析小数部分
            if (i < n && expr[i] == '.') {
                i++;
                double fraction = 0.1;
                while (i < n && isdigit(expr[i])) {
                    num += (expr[i] - '0') * fraction;
                    fraction *= 0.1;
                    i++;
                }
            }
            numStack.push(num);
        }
        // 如果是左括号，直接入栈
        else if (expr[i] == '(') {
            opStack.push(expr[i]);
            i++;
        }
        // 如果是右括号，处理直到遇到左括号
        else if (expr[i] == ')') {
            while (opStack.getTop() != '(') {
                char op = opStack.pop();
                double num2 = numStack.pop();
                double num1 = numStack.pop();
                numStack.push(calculate(num1, num2, op));
            }
            opStack.pop(); // 弹出左括号
            i++;
        }
        // 如果是运算符
        else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/' || expr[i] == '^') {
            // 当前运算符与栈顶运算符比较优先级
            while (!opStack.isEmpty() && getPriority(opStack.getTop()) >= getPriority(expr[i])) {
                char op = opStack.pop();
                double num2 = numStack.pop();
                double num1 = numStack.pop();
                numStack.push(calculate(num1, num2, op));
            }
            opStack.push(expr[i]);
            i++;
        }
        // 未知字符
        else {
            throw runtime_error("表达式中包含无效字符: " + string(1, expr[i]));
        }
    }
    
    // 处理剩余的运算符
    while (!opStack.isEmpty()) {
        char op = opStack.pop();
        double num2 = numStack.pop();
        double num1 = numStack.pop();
        numStack.push(calculate(num1, num2, op));
    }
    
    return numStack.getTop();
}

// 测试案例
void testCalculator() {
    // 基本运算测试
    try {
        string expr1 = "3 + 4 * 2 / (1 - 5)";
        double result1 = evaluateExpression(expr1);
        cout << expr1 << " = " << result1 << endl; // 预期结果: 1
    } catch (const exception& e) {
        cout << "表达式 \"" << "3 + 4 * 2 / (1 - 5)" << "\" 错误: " << e.what() << endl;
    }
    
    // 带括号的运算
    try {
        string expr2 = "((8 + 3) * 2 - 5) / 3";
        double result2 = evaluateExpression(expr2);
        cout << expr2 << " = " << result2 << endl; // 预期结果: 5.666...
    } catch (const exception& e) {
        cout << "表达式 \"" << "((8 + 3) * 2 - 5) / 3" << "\" 错误: " << e.what() << endl;
    }
    
    // 幂运算测试
    try {
        string expr3 = "2 ^ 3 + 5";
        double result3 = evaluateExpression(expr3);
        cout << expr3 << " = " << result3 << endl; // 预期结果: 13
    } catch (const exception& e) {
        cout << "表达式 \"" << "2 ^ 3 + 5" << "\" 错误: " << e.what() << endl;
    }
    
    // 小数运算测试
    try {
        string expr4 = "1.5 + 2.5 * 3";
        double result4 = evaluateExpression(expr4);
        cout << expr4 << " = " << result4 << endl; // 预期结果: 9
    } catch (const exception& e) {
        cout << "表达式 \"" << "1.5 + 2.5 * 3" << "\" 错误: " << e.what() << endl;
    }
    
    // 无效表达式测试
    try {
        string expr5 = "3 + / 5";
        double result5 = evaluateExpression(expr5);
        cout << expr5 << " = " << result5 << endl;
    } catch (const exception& e) {
        cout << "表达式 \"" << "3 + / 5" << "\" 错误: " << e.what() << endl;
    }
    
    // 括号不匹配测试
    try {
        string expr6 = "(3 + 5 * 2";
        double result6 = evaluateExpression(expr6);
        cout << expr6 << " = " << result6 << endl;
    } catch (const exception& e) {
        cout << "表达式 \"" << "(3 + 5 * 2" << "\" 错误: " << e.what() << endl;
    }
    
    // 除法测试
    try {
        string expr7 = "10 / (2 + 3)";
        double result7 = evaluateExpression(expr7);
        cout << expr7 << " = " << result7 << endl; // 预期结果: 2
    } catch (const exception& e) {
        cout << "表达式 \"" << "10 / (2 + 3)" << "\" 错误: " << e.what() << endl;
    }
    
    // 除以零测试
    try {
        string expr8 = "5 / (3 - 3)";
        double result8 = evaluateExpression(expr8);
        cout << expr8 << " = " << result8 << endl;
    } catch (const exception& e) {
        cout << "表达式 \"" << "5 / (3 - 3)" << "\" 错误: " << e.what() << endl;
    }
    
    // 三角函数测试
    try {
        string expr9 = "sin(3.1415926535 / 2)";
        double result9 = evaluateExpression(expr9);
        cout << expr9 << " = " << result9 << endl; // 预期结果: 约1.0
    } catch (const exception& e) {
        cout << "表达式 \"" << "sin(3.1415926535 / 2)" << "\" 错误: " << e.what() << endl;
    }
    
    // 复杂函数测试
    try {
        string expr10 = "sqrt(16) + log(exp(1))";
        double result10 = evaluateExpression(expr10);
        cout << expr10 << " = " << result10 << endl; // 预期结果: 4 + 1 = 5
    } catch (const exception& e) {
        cout << "表达式 \"" << "sqrt(16) + log(exp(1))" << "\" 错误: " << e.what() << endl;
    }
}

int main() {
    cout << "字符串计算器测试:" << endl;
    testCalculator();
    
    // 交互式计算
    string expr;
    cout << "\n请输入表达式(输入q退出): ";
    while (getline(cin, expr) && expr != "q") {
        try {
            double result = evaluateExpression(expr);
            cout << "结果: " << result << endl;
        } catch (const exception& e) {
            cout << "错误: " << e.what() << endl;
        }
        cout << "请输入表达式(输入q退出): ";
    }
    
    return 0;
}
    
