#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

class Transaction {
private:
    time_t timestamp;
    double amount;
    string description;

public:
    Transaction(double amt, const string& desc) : amount(amt), description(desc) {
        timestamp = time(nullptr);
    }

    void displayTransaction() const {
        cout << "Timestamp: " << asctime(localtime(&timestamp));
        cout << "Amount: $" << amount << endl;
        cout << "Description: " << description << endl;
    }
};

class Account {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;
    vector<Transaction> transactions;

public:
    Account(const string& accNumber, const string& holderName, double initialBalance) :
        accountNumber(accNumber), accountHolderName(holderName), balance(initialBalance) {}

    virtual void deposit(double amount) {
        balance += amount;
        transactions.push_back(Transaction(amount, "Deposit"));
        cout << "Deposited $" << amount << ". New balance: $" << balance << endl;
    }

    virtual void withdraw(double amount) = 0;

    virtual void transfer(Account& receiver, double amount) {
        if (balance >= amount) {
            balance -= amount;
            receiver.deposit(amount);
            transactions.push_back(Transaction(amount, "Transfer to " + receiver.getAccountNumber()));
            cout << "Transferred $" << amount << " to account " << receiver.getAccountNumber() << endl;
        } else {
            cout << "Insufficient balance for transfer. Transaction failed." << endl;
        }
    }

    virtual void displayInfo() const {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Account Holder: " << accountHolderName << endl;
        cout << "Balance: $" << balance << endl;
    }

    void displayTransactions() const {
        cout << "Transaction History for Account " << accountNumber << ":" << endl;
        for (const auto& transaction : transactions) {
            transaction.displayTransaction();
        }
    }

    string getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    virtual void closeAccount() {
        transactions.clear();
        balance = 0.0;
        cout << "Account " << accountNumber << " closed. Balance refunded: $" << balance << endl;
    }

    virtual bool hasHolderName(const string& name) const {
        return accountHolderName == name;
    }

    virtual ~Account() {}
};

class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const string& accNumber, const string& holderName, double initialBalance, double interest) :
        Account(accNumber, holderName, initialBalance), interestRate(interest) {}

    void deposit(double amount) override {
        double interestEarned = amount * (interestRate / 100.0);
        balance += amount + interestEarned;
        transactions.push_back(Transaction(amount + interestEarned, "Deposit with interest"));
        cout << "Deposited $" << amount << " with interest $" << interestEarned << ". New balance: $" << balance << endl;
    }

    void withdraw(double amount) override {
        if (balance >= amount) {
            balance -= amount;
            transactions.push_back(Transaction(amount, "Withdrawal"));
            cout << "Withdrawn $" << amount << ". New balance: $" << balance << endl;
        } else {
            cout << "Insufficient balance for withdrawal. Transaction failed." << endl;
        }
    }

    void displayInfo() const override {
        cout << "Savings Account Details:" << endl;
        Account::displayInfo();
        cout << "Interest Rate: " << interestRate << "%" << endl;
    }
};

class CheckingAccount : public Account {
    double overdraftLimit;

public:
    CheckingAccount(const string& accNumber, const string& holderName, double initialBalance, double overdraft) :
        Account(accNumber, holderName, initialBalance), overdraftLimit(overdraft) {}

    void withdraw(double amount) override {
        if (balance + overdraftLimit >= amount) {
            balance -= amount;
            transactions.push_back(Transaction(amount, "Withdrawal"));
            cout << "Withdrawn $" << amount << ". New balance: $" << balance << endl;
        } else {
            cout << "Withdrawal exceeds overdraft limit. Transaction failed." << endl;
        }
    }

    void displayInfo() const override {
        cout << "Checking Account Details:" << endl;
        Account::displayInfo();
        cout << "Overdraft Limit: $" << overdraftLimit << endl;
    }

    bool hasHolderName(const string& name) const override {
        return accountHolderName == name || accountHolderName.find(name) != string::npos;
    }
};

class Bank {
private:
    vector<Account*> accounts;

public:
    void addAccount(Account* acc) {
        accounts.push_back(acc);
        cout << "Account added successfully.\n";
    }

    void displayAllAccounts() const {
        cout << "All Accounts in the Bank:\n";
        cout << "-------------------------------------\n";
        for (const auto& acc : accounts) {
            acc->displayInfo();
            cout << "-------------------------------------\n";
        }
    }

    Account* findAccount(const string& accNumber) const {
        for (const auto& acc : accounts) {
            if (acc->getAccountNumber() == accNumber) {
                return acc;
            }
        }
        return nullptr;
    }

    vector<Account*> findAccountsByHolderName(const string& name) const {
        vector<Account*> matchingAccounts;
        for (const auto& acc : accounts) {
            if (acc->hasHolderName(name)) {
                matchingAccounts.push_back(acc);
            }
        }
        return matchingAccounts;
    }

    void closeAccount(const string& accNumber) {
        for (auto it = accounts.begin(); it != accounts.end(); ++it) {
            if ((*it)->getAccountNumber() == accNumber) {
                (*it)->closeAccount();
                accounts.erase(it);
                cout << "Account " << accNumber << " removed from the bank.\n";
                return;
            }
        }
        cout << "Account not found.\n";
    }

    double getTotalBalance() const {
        double total = 0.0;
        for (const auto& acc : accounts) {
            total += acc->getBalance();
        }
        return total;
    }
};
