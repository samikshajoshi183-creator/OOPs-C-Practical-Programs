#include <iostream>
using namespace std;

// Base Class
class Person {
public:
    string name;

    void getPerson() {
        cout << "Enter Name: ";
        cin >> name;
    }

    void displayPerson() {
        cout << "Name: " << name << endl;
    }
};

// Single Inheritance
class Student : public Person {
public:
    int rollNo;

    void getStudent() {
        cout << "Enter Roll Number: ";
        cin >> rollNo;
    }

    void displayStudent() {
        displayPerson();
        cout << "Roll Number: " << rollNo << endl;
    }
};

// Multilevel Inheritance
class Result : public Student {
public:
    int marks;

    void getResult() {
        cout << "Enter Marks: ";
        cin >> marks;
    }

    void displayResult() {
        displayStudent();
        cout << "Marks: " << marks << endl;
    }
};

int main() {
    Result r;

    r.getPerson();
    r.getStudent();
    r.getResult();

    cout << "\nStudent Details:\n";
    r.displayResult();

    return 0;
}
