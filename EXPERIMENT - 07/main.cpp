#include <iostream>
using namespace std;

class ComplexNumber {
    double real, imag;

public:
    ComplexNumber(double r = 0, double i = 0) {
        real = r;
        imag = i;
    }

    ComplexNumber add(ComplexNumber c) {
        return ComplexNumber(real + c.real, imag + c.imag);
    }

    ComplexNumber operator+(ComplexNumber c) {
        return ComplexNumber(real + c.real, imag + c.imag);
    }

    void display() {
        cout << real << " + " << imag << "i" << endl;
    }
};

// Function overloading
int add(int a, int b) {
    return a + b;
}

double add(double a, double b) {
    return a + b;
}

int main() {
    cout << "Integer Addition: " << add(10, 20) << endl;
    cout << "Double Addition: " << add(5.5, 2.5) << endl;

    ComplexNumber c1(3, 4), c2(2, 5);

    ComplexNumber c3 = c1 + c2;

    cout << "Complex Number Addition: ";
    c3.display();

    return 0;
}
