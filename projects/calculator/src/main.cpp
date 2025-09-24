#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <functional>
#include <stdexcept>
#include <memory>
#include <map>

// RAII: Base class for expression tree nodes.
class Node {
public:
    virtual double evaluate() const = 0;
    virtual ~Node() = default;
};

// RAII: Node to represent a number.
class NumberNode : public Node {
public:
    // `auto` used here for parameter type deduction (a more common use is with iterators).
    explicit NumberNode(auto val) : value(val) {}
    double evaluate() const override { return value; }
private:
    double value;
};

// RAII: Node to represent a binary operation.
class OperationNode : public Node {
public:
    // We use std::unique_ptr for RAII, ensuring child nodes are automatically deleted.
    OperationNode(char op, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : op_char(op), left_child(std::move(left)), right_child(std::move(right)) {}

    double evaluate() const override {
        // `auto` used to automatically deduce the type of the evaluated children.
        auto left_val = left_child->evaluate();
        auto right_val = right_child->evaluate();

        // Lambda function to perform the operation.
        // We capture the operator character by value.
        auto operation = [this, left_val, right_val]() -> double {
            switch (op_char) {
                case '+': return left_val + right_val;
                case '-': return left_val - right_val;
                case '*': return left_val * right_val;
                case '/':
                    if (right_val == 0.0) {
                        throw std::runtime_error("Division by zero!");
                    }
                    return left_val / right_val;
                default:
                    throw std::runtime_error("Unknown operator");
            }
        };

        // We use decltype here to get the return type of the lambda.
        decltype(operation()) result = operation();
        return result;
    }
private:
    char op_char;
    std::unique_ptr<Node> left_child;
    std::unique_ptr<Node> right_child;
};

// Function Object (Functor) to define operator precedence.
struct OperatorPrecedence {
    std::map<char, int> precedence_map;
    // Constructor to initialize the map.
    OperatorPrecedence() {
        precedence_map['+'] = 1;
        precedence_map['-'] = 1;
        precedence_map['*'] = 2;
        precedence_map['/'] = 2;
    }
    // Overload the function call operator `()`
    // to check the precedence of an operator.
    int operator()(char op) const {
        auto it = precedence_map.find(op);
        if (it != precedence_map.end()) {
            return it->second;
        }
        return 0; // Low precedence for unknown operators.
    }
};

// Function to check if a character is an operator.
bool is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

// Parses an infix expression and builds an expression tree.
std::unique_ptr<Node> parse_expression(const std::string& expression) {
    std::stack<std::unique_ptr<Node>> values;
    std::stack<char> ops;
    OperatorPrecedence get_precedence;

    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (isspace(c)) {
            continue;
        } else if (isdigit(c)) {
            std::string num_str;
            while (i < expression.length() && (isdigit(expression[i] || expression[i] == '.'))) {
                num_str += expression[i];
                i++;
            }
            i--;
            values.push(std::make_unique<NumberNode>(std::stod(num_str)));
        } else if (c == '(') {
            ops.push(c);
        } else if (c == ')') {
            while (!ops.empty() && ops.top() != '(') {
                char op = ops.top();
                ops.pop();
                std::unique_ptr<Node> right = std::move(values.top());
                values.pop();
                std::unique_ptr<Node> left = std::move(values.top());
                values.pop();
                values.push(std::make_unique<OperationNode>(op, std::move(left), std::move(right)));
            }
            if (!ops.empty()) {
                ops.pop(); // Pop '('
            }
        } else if (is_operator(c)) {
            while (!ops.empty() && ops.top() != '(' && get_precedence(ops.top()) >= get_precedence(c)) {
                char op = ops.top();
                ops.pop();
                std::unique_ptr<Node> right = std::move(values.top());
                values.pop();
                std::unique_ptr<Node> left = std::move(values.top());
                values.pop();
                values.push(std::make_unique<OperationNode>(op, std::move(left), std::move(right)));
            }
            ops.push(c);
        }
    }

    while (!ops.empty()) {
        char op = ops.top();
        ops.pop();
        std::unique_ptr<Node> right = std::move(values.top());
        values.pop();
        std::unique_ptr<Node> left = std::move(values.top());
        values.pop();
        values.push(std::make_unique<OperationNode>(op, std::move(left), std::move(right)));
    }

    return std::move(values.top());
}

int main() {
    std::cout << "C++ Expression Calculator REPL" << std::endl;
    std::cout << "Enter an expression (e.g., 2 + 3 * (4 - 1)) or 'quit' to exit." << std::endl;

    // RAII for input loop
    for (std::string line; std::getline(std::cin, line); ) {
        if (line == "quit") {
            break;
        }

        try {
            // The `unique_ptr` here will automatically manage the memory for the entire tree.
            std::unique_ptr<Node> root_node = parse_expression(line);

            // `auto` simplifies the type of the result variable.
            auto result = root_node->evaluate();
            std::cout << "Result: " << result << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
