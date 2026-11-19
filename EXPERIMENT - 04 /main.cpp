#include <iostream>
#include <memory>
using namespace std;

class Student {
public:
    string name;

    Student(string n) {
        name = n;
        cout << name << " object created" << endl;
    }

    void display() {
        cout << "Student Name: " << name << endl;
    }

    ~Student() {
        cout << name << " object destroyed" << endl;
    }
};

int main() {

    // Example 1: unique_ptr
    cout << "Using unique_ptr:" << endl;

    unique_ptr<Student> s1 =
        make_unique<Student>("Rahul");

    s1->display();

    cout << "Unique pointer has one owner." << endl;

    // Example 2: shared_ptr
    cout << "\nUsing shared_ptr:" << endl;

    shared_ptr<Student> s2 =
        make_shared<Student>("Amit");

    cout << "Number of owners: " << s2.use_count() << endl;

    {
        shared_ptr<Student> s3 = s2;

        s3->display();

        cout << "Number of owners: "
             << s2.use_count() << endl;
    }

    cout << "After s3 is destroyed:" << endl;
    cout << "Number of owners: "
         << s2.use_count() << endl;

    return 0;
}
