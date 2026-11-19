#include <iostream>
using namespace std;

int main() {
    int a, b;
    const int value = 10;

    cout << "Enter first number: ";
    cin >> a;

    cout << "Enter second number: ";
    cin >> b;

    // Arithmetic Operators
    cout << "\nArithmetic Operations:" << endl;
    cout << "Addition: " << a + b << endl;
    cout << "Subtraction: " << a - b << endl;
    cout << "Multiplication: " << a * b << endl;

    if (b != 0) {
        cout << "Division: " << (float)a / b << endl;
    } else {
        cout << "Division not possible" << endl;
    }

    // Relational Operator
    cout << "\nRelational Operation:" << endl;
    cout << "Is a greater than b? " << (a > b) << endl;

    // Logical Operator
    cout << "\nLogical Operation:" << endl;
    cout << "Both numbers are positive: "
         << (a > 0 && b > 0) << endl;

    // Constant
    cout << "\nConstant Value: " << value << endl;

    return 0;
}
