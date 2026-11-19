#include <iostream>
#include <stack>
#include <string>
using namespace std;

template <class T>
class Stack {
    stack<T> s;

public:
    void push(T value) {
        s.push(value);
    }

    void pop() {
        if (s.empty()) {
            cout << "Stack is Empty!" << endl;
        } else {
            s.pop();
        }
    }

    void displayTop() {
        if (s.empty()) {
            cout << "Stack is Empty!" << endl;
        } else {
            cout << "Top Element: " << s.top() << endl;
        }
    }

    bool isEmpty() {
        return s.empty();
    }
};

int main() {
    Stack<int> s1;
    s1.push(10);
    s1.push(20);
    s1.displayTop();

    Stack<float> s2;
    s2.push(10.5);
    s2.push(20.5);
    s2.displayTop();

    Stack<string> s3;
    s3.push("Hello");
    s3.push("C++");
    s3.displayTop();

    return 0;
}
