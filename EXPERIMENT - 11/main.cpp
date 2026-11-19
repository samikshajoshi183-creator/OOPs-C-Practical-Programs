#include <iostream>
#include <fstream>
#include <string>
using namespace std;

class Student {
public:
    int id;
    string name;
    float marks;

    void input() {
        cout << "Enter Student ID: ";
        cin >> id;

        cout << "Enter Name: ";
        cin >> name;

        cout << "Enter Marks: ";
        cin >> marks;
    }

    void display() {
        cout << id << "\t" << name << "\t" << marks << endl;
    }
};

int main() {
    Student s;
    ofstream fout("students.csv");

    if (!fout) {
        cout << "File cannot be opened!" << endl;
        return 1;
    }

    fout << "ID,Name,Marks\n";

    for (int i = 0; i < 3; i++) {
        cout << "\nEnter Student " << i + 1 << " Details\n";
        s.input();

        fout << s.id << "," << s.name << "," << s.marks << "\n";
    }

    fout.close();

    cout << "\nStudent Records:\n";

    ifstream fin("students.csv");
    string line;

    while (getline(fin, line)) {
        cout << line << endl;
    }

    fin.close();

    // Demonstration of seekg() and tellg()
    ifstream file("students.csv");

    file.seekg(0, ios::end);
    cout << "\nFile Size: " << file.tellg() << " bytes" << endl;

    file.seekg(0, ios::beg);
    cout << "Reading Position: " << file.tellg() << endl;

    file.close();

    // Demonstration of seekp() and tellp()
    fstream outfile("position.txt", ios::in | ios::out | ios::trunc);

    outfile << "Hello C++";
    cout << "\nWrite Position: " << outfile.tellp() << endl;

    outfile.seekp(6, ios::beg);
    cout << "New Write Position: " << outfile.tellp() << endl;

    outfile.close();

    cout << "\nFile Operations Completed!" << endl;

    return 0;
}
