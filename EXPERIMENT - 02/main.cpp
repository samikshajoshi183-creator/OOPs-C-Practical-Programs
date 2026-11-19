#include <iostream>
using namespace std;

class BankAccount {
private:
    int accountNumber;
    float balance;

public:
    // Constructor
    BankAccount(int acc, float bal) {
        accountNumber = acc;

        if (bal >= 0)
            balance = bal;
        else
            balance = 0;
    }

    // Deposit function
    void deposit(float amount) {
        if (amount > 0) {
            balance = balance + amount;
            cout << "Amount Deposited Successfully" << endl;
        } else {
            cout << "Invalid Amount" << endl;
        }
    }

    // Display function
    void display() {
        cout << "\nAccount Number: " << accountNumber << endl;
        cout << "Balance: " << balance << endl;
    }
};

int main() {
    BankAccount b1(101, 5000);
    BankAccount b2(102, 3000);

    cout << "First Account Details:";
    b1.display();

    cout << "\nSecond Account Details:";
    b2.display();

    cout << "\nDepositing Rs. 1000 in First Account" << endl;
    b1.deposit(1000);

    cout << "\nUpdated First Account Details:";
    b1.display();

    return 0;
}
