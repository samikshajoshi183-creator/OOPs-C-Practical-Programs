#include <iostream>
using namespace std;

// Abstract Base Class
class Shape {
public:
    virtual void area() = 0;
};

// Derived Class 1
class Circle : public Shape {
private:
    float radius;

public:
    Circle(float r) {
        radius = r;
    }

    void area() override {
        cout << "Area of Circle: "
             << 3.14 * radius * radius << endl;
    }
};

// Derived Class 2
class Rectangle : public Shape {
private:
    float length, breadth;

public:
    Rectangle(float l, float b) {
        length = l;
        breadth = b;
    }

    void area() override {
        cout << "Area of Rectangle: "
             << length * breadth << endl;
    }
};

// Main Function
int main() {
    Circle c(5);
    Rectangle r(4, 6);

    Shape *ptr;

    ptr = &c;
    ptr->area();

    ptr = &r;
    ptr->area();

    return 0;
}
