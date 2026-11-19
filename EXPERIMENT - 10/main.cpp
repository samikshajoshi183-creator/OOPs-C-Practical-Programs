#include <iostream>
#include <exception>
using namespace std;

class InvalidAmount : public exception {
public:
    const char* what() const noexcept override {
        return "Invalid Amount!";
    }
};

class InsufficientFunds : public exception {
public:
    const char* what() const noexcept override {
        return "Insufficient Balance!";
    }
};

class BankAccount {
    double balance;

public:
    BankAccount(double b) {
        balance = b;
    }

    void deposit(double amount) {
        if (amount <= 0)
            throw InvalidAmount();

        balance += amount;
        cout << "Amount Deposited Successfully!" << endl;
    }

    void withdraw(double amount) {
        if (amount <= 0)
            throw InvalidAmount();

        if (amount > balance)
            throw InsufficientFunds();

        balance -= amount;
        cout << "Withdrawal Successful!" << endl;
    }

    void display() {
        cout << "Current Balance: " << balance << endl;
    }
};

int main() {
    BankAccount account(5000);

    try {
        account.deposit(1000);
        account.withdraw(2000);
        account.withdraw(10000);
    }
    catch (const InvalidAmount& e) {
        cout << e.what() << endl;
    }
    catch (const InsufficientFunds& e) {
        cout << e.what() << endl;
    }

    account.display();

    return 0;
}
