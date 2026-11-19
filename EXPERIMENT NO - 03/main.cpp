#include <iostream>
#include <utility>
using namespace std;

class Array {
private:
    int size;
    int *data;

public:
    // 1. Default Constructor
    Array() {
        size = 0;
        data = nullptr;
        cout << "Default Constructor Called" << endl;
    }

    // 2. Parameterized Constructor
    Array(int n) {
        size = n;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = i + 1;
        }

        cout << "Parameterized Constructor Called" << endl;
    }

    // 3. Copy Constructor (Deep Copy)
    Array(const Array &obj) {
        size = obj.size;
        data = new int[size];

        for (int i = 0; i < size; i++) {
            data[i] = obj.data[i];
        }

        cout << "Copy Constructor Called" << endl;
    }

    // 4. Copy Assignment Operator
    Array& operator=(const Array &obj) {
        if (this != &obj) {
            delete[] data;

            size = obj.size;
            data = new int[size];

            for (int i = 0; i < size; i++) {
                data[i] = obj.data[i];
            }
        }

        cout << "Copy Assignment Called" << endl;
        return *this;
    }

    // 5. Move Constructor
    Array(Array &&obj) noexcept {
        size = obj.size;
        data = obj.data;

        obj.size = 0;
        obj.data = nullptr;

        cout << "Move Constructor Called" << endl;
    }

    // 6. Move Assignment Operator
    Array& operator=(Array &&obj) noexcept {
        if (this != &obj) {
            delete[] data;

            size = obj.size;
            data = obj.data;

            obj.size = 0;
            obj.data = nullptr;
        }

        cout << "Move Assignment Called" << endl;
        return *this;
    }

    // Display Function
    void display() {
        for (int i = 0; i < size; i++) {
            cout << data[i] << " ";
        }
        cout << endl;
    }

    // Destructor
    ~Array() {
        delete[] data;
    }
};

int main() {

    // Default Constructor
    Array a1;

    // Parameterized Constructor
    Array a2(3);
    cout << "Original Array: ";
    a2.display();

    // Copy Constructor
    Array a3(a2);
    cout << "Copied Array: ";
    a3.display();

    // Copy Assignment
    Array a4(2);
    a4 = a2;
    cout << "Copy Assigned Array: ";
    a4.display();

    // Move Constructor
    Array a5(move(a2));
    cout << "Moved Array: ";
    a5.display();

    // Move Assignment
    Array a6(1);
    a6 = move(a4);
    cout << "Move Assigned Array: ";
    a6.display();

    return 0;
}
