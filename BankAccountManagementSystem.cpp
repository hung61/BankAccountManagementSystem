#include <iostream>
#include <vector>
#include <string>
using namespace std;

// Class Transaction: stores information about a transaction
class Transaction {
    private:
        double amount; // Transaction amount
        string type; // Type of transaction
        string date; // Transaction date

    public:
        // Constructor: receives the amount, transaction type, and date
        Transaction(double _amount, string _type, string _date): amount(_amount), type(_type), date(_date) {}
};

// Class Account: manages the information and transactions of an account
class Account {
    protected:
        string accountNumber; // Account number
        double balance; // Account balance
        string ownerName; // Account holder's name
        vector<Transaction> transactionHistory; // Transaction history

    public:
        // Constructor: receives the account number, balance, owner name, and transaction history
        Account(string _accountNumber, double _balance, string _ownerName, vector<Transaction> _transactionHistory): 
        accountNumber(_accountNumber), balance(_balance), ownerName(_ownerName), transactionHistory(_transactionHistory) {}

        // Return the account number
        string getAccountNumber() {return accountNumber;}

        // Return the current balance
        double getBalance() {return balance;}

        // Update the account balance
        void setBalance(double _balance) {balance = _balance;}

        // Display the account number and current balance
        void balanceInquiry() {
            cout << "Account number: " << accountNumber << endl;
            cout << "Current balance: " << balance << " VND" << endl;
        }

        // Operator += : add a new transaction to the history
        Account &operator+=(const Transaction transaction) {
            transactionHistory.push_back(transaction);
            return *this;
        }

        // Deposit money into the account
        void deposit(string date) {
            cout << "Enter the amount you want to deposit: ";
            double amount;
            cin >> amount;
            if (amount >= 0) {
                cout << "Deposit successful!\n";
                balance += amount;

                Transaction newTransaction(amount, "Deposit", date);
                *this += newTransaction;
                balanceInquiry();
            } else cout << "Invalid\n";
        }

        // Withdraw money from the account
        virtual void withdraw(string date) {
            cout << "Enter the amount you want to withdraw: ";
            double amount;
            cin >> amount;

            if (amount >= 0) {
                if (amount > balance) {
                    cout << "Insufficient balance!" << endl;
                } else {
                    cout << "Withdraw successful\n";
                    balance -= amount;

                    Transaction newTransaction(-amount, "Withdraw", date);
                    *this += newTransaction;
                    balanceInquiry();
                }
            } else cout << "Invalid\n";
        }

        // Comparison operator == : check if two accounts have the same balance
        bool operator==(const Account &other) {
            return this->balance == other.balance;
        }

        // Compare the balance of this account with another account
        void compareAccount(Account account2) {
            if (*this == account2) cout << "The two accounts have the same balance" << endl;
            else cout << "The two accounts don't have the same balance" << endl;
        }
};

// Class SavingsAccount: inherits from Account, adds interest rate and minimum balance
class SavingsAccount: public Account {
    private:
        double interestRate; // Interest rate (%)
        double minBalance = 100000; // Minimum balance to maintain

    public:
        // Constructor: takes account number, balance, owner name, interest rate, and transaction history
        SavingsAccount(string _accountNumber, double _balance, string _ownerName, double _interestRate, vector<Transaction> _transactionHistory)
        : Account(_accountNumber, _balance, _ownerName, _transactionHistory), interestRate(_interestRate) {}

        // Withdraw money (add interest before withdrawal and check minimum balance)
        void withdraw(string date) override {
            cout << "Current balance (before interest): " << balance << " VND\n";

            double interest = balance * (interestRate / 100.0); // Calculate interest
            balance += interest; // Add interest to balance
            cout << "Interest added: " << interest << " VND\n";
            cout << "Balance after interest: " << getBalance() << " VND\n"; // Show balance after interest

            cout << "Enter the amount you want to withdraw: ";
            double amount; 
            cin >> amount;

            if (amount <= 0) { // Validate the entered amount
                cout << "Invalid amount!\n";
                return;
            }
            if (amount > balance - minBalance) { // Check if withdrawal keeps the minimum balance
                cout << "You must keep at least " << minBalance << " VND.\n";
                return;
            }

            balance -= amount; // Deduct money after withdrawal
            Transaction t(-amount, "Withdraw", date); // Add withdrawal transaction to history
            *this += t;

            cout << "Withdraw successful.\n";
            balanceInquiry(); // Display remaining balance
        }
};

// Class Customer: manages customer information, list of regular and savings accounts
class Customer {
    private:
        string name; // Customer name
        string ID; // Customer ID
        vector<Account> ownedAccounts; // List of regular accounts
        vector<SavingsAccount> ownedSavingsAccounts; // List of savings accounts

    public:
        // Constructor: takes personal info and account lists
        Customer(string _name, string _ID, vector<Account> _ownedAccounts, vector<SavingsAccount> _ownedSavingsAccounts)
        : name(_name), ID(_ID), ownedAccounts(_ownedAccounts), ownedSavingsAccounts(_ownedSavingsAccounts) {}

        // Get reference to list of regular accounts
        vector<Account>& getOwnedAccounts() { return ownedAccounts; }

        // Get reference to list of savings accounts
        vector<SavingsAccount>& getOwnedSavingsAccounts() { return ownedSavingsAccounts; }
        
        // Display customer information
        void displayInfo() {
            cout << "Name: " << name << endl;
            cout << "ID: " << ID << endl;
        }
        
        // Open a new account
        void openNewAccount() {
            cout << "Enter account type (Regular / Savings): ";
            string accountType; 
            cin >> accountType;
            
            if (accountType == "Regular" || accountType == "Savings") {
                cout << "Enter account number: ";
                string accountNumber; 
                cin >> accountNumber;
                cin.ignore();
                cout << "Enter owner name: ";
                string ownerName; 
                getline(cin, ownerName);

                double interestRate;
                if (accountType == "Savings") { // If opening a savings account, input interest rate
                    cout << "Enter interest rate: ";
                    cin >> interestRate;
                }

                cout << "Open new account successful!\n\n";

                if (accountType == "Regular") { 
                    // Create a regular account
                    Account newAcc(accountNumber, 0, ownerName, {});
                    ownedAccounts.push_back(newAcc);
                    newAcc.balanceInquiry();
                } else { 
                    // Create a savings account
                    SavingsAccount newAcc(accountNumber, 0, ownerName, interestRate, {});
                    ownedSavingsAccounts.push_back(newAcc);
                    newAcc.balanceInquiry();
                }
            } else cout << "Invalid\n";
        }

        // Calculate total balance of all accounts
        void calculateTotalBalance() {
            double total = 0;

            // Add balances of regular accounts
            for (int i = 0; i < ownedAccounts.size(); i++) {
                total += ownedAccounts[i].getBalance();
            }
            // Add balances of savings accounts
            for (int i = 0; i < ownedSavingsAccounts.size(); i++) {
                total += ownedSavingsAccounts[i].getBalance();
            }

            cout << "Total Balance: " << total << endl; // Display total balance

            // Display details of each regular account
            cout << "Regular Accounts:\n";
            for (int i = 0; i < ownedAccounts.size(); i++) {
                ownedAccounts[i].balanceInquiry();
                cout << endl;
            }

            // Display details of each savings account
            cout << "Savings Accounts:\n";
            for (int i = 0; i < ownedSavingsAccounts.size(); i++) {
                ownedSavingsAccounts[i].balanceInquiry();
                cout << endl;
            }
        }

        void transfer(string date){
            int maxOption = 1;
            bool src_case2 = false;
            if (ownedAccounts.size() > 0){
                cout << "Choose type of source account:\n";
                cout << "1. Regular\n";
                if (ownedSavingsAccounts.size() > 0){
                    cout << "2. Savings\n";
                    maxOption = 2;
                }

            } else if (ownedSavingsAccounts.size() > 0){
                bool src_case2 = true;
                cout << "Choose type of source account:\n";
                cout << "1. Savings\n";
            }
            cout << "Choose: ";
            int n1; cin >> n1;
            if (src_case2) n1++;
            cout << endl;

            if (n1 == 1){
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < ownedAccounts.size(); i++){
                    cout << i + 1 << ". " << ownedAccounts[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int m1; cin >> m1;
                if (m1 < 1 || m1 > ownedAccounts.size()){
                    cout << "Invalid\n";
                    return;
                }
                cout << endl;

                cout << "Choose type of destination account:\n";
                int maxDestOption = 1;
                bool dest_case2 = false;

                vector<Account> ownedAccounts_draft = ownedAccounts;
                ownedAccounts_draft.erase(ownedAccounts_draft.begin() + m1 - 1);

                if (ownedAccounts_draft.size() > 0) {
                    cout << "1. Regular\n";
                    if (ownedSavingsAccounts.size() > 0) {
                        cout << "2. Savings\n";
                        maxDestOption = 2;
                    }
                } else if (ownedSavingsAccounts.size() > 0) {
                    dest_case2 = true;
                    cout << "1. Savings\n";
                } else {
                    cout << "No destination accounts are available!\n";
                    return;
                }

                cout << "Choose: ";
                int n2;
                cin >> n2;
                if (dest_case2) n2++;   
                cout << endl;

                if (n2 == 1){
                    cout << "Choose one of account numbers below:\n";
                    int order = 0;
                    vector<int> idxOfDesAccounts;
                    for (int i = 0; i < ownedAccounts.size(); i++){
                        if (i != m1 - 1){
                            order++;
                            idxOfDesAccounts.push_back(i);
                            cout << order << ". " << ownedAccounts[i].getAccountNumber() << endl;
                        }
                    }

                    cout << "Choose: ";
                    int m2; cin >> m2;
                    if (m2 < 1 || m2 > (ownedAccounts.size() - 1)){
                        cout << "Invalid\n";
                        return;
                    }
                    int desIdx = idxOfDesAccounts[m2 - 1];
                    cout << endl;

                    cout << "Enter amount of money you want to transfer: ";
                    double amount; cin >> amount;

                    if (amount <= 0){
                        cout << "Invalid\n";
                        return;
                    } else if (amount > ownedAccounts[m1 - 1].getBalance()){
                        cout << "Insufficient balance!";
                        return;
                    }
                    cout << "Transfer sucessful!\n\n";
                    
                    ownedAccounts[m1 - 1].setBalance(ownedAccounts[m1 - 1].getBalance() - amount);
                    ownedAccounts[desIdx].setBalance(ownedAccounts[desIdx].getBalance() + amount);
                    ownedAccounts[desIdx].balanceInquiry();

                    Transaction newTransaction1(-amount, "Transfer", date);
                    ownedAccounts[m1 - 1] += newTransaction1;
                    Transaction newTransaction2(amount, "Transfer", date);
                    ownedAccounts[m2 - 1] += newTransaction2;
                } else if ((n2 == 2 && dest_case2) || (n2 == 2 && n2 == maxDestOption)){
                    cout << "Choose one of account numbers below:\n";
                    for (int i = 0; i < ownedSavingsAccounts.size(); i++){
                        cout << i + 1 << ". " << ownedSavingsAccounts[i].getAccountNumber() << endl;
                    }
                    
                    cout << "Choose: ";
                    int m2; cin >> m2;
                    if (m2 < 1 || m2 > ownedSavingsAccounts.size()){
                        cout << "Invalid\n";
                        return;
                    }
                    cout << endl;

                    cout << "Enter amount of money you want to transfer: ";
                    double amount; cin >> amount;
                    if (amount <= 0){
                        cout << "Invalid\n";
                        return;
                    } else if (amount > ownedAccounts[m1 - 1].getBalance()){
                        cout << "Insufficient balance!";
                        return;
                    }
                    cout << "Transfer sucessful!\n\n";

                    ownedAccounts[m1 - 1].setBalance(ownedAccounts[m1 - 1].getBalance() - amount);
                    ownedSavingsAccounts[m2 - 1].setBalance(ownedSavingsAccounts[m2 - 1].getBalance() + amount);
                    ownedSavingsAccounts[m2 - 1].balanceInquiry();

                    Transaction newTransaction1(-amount, "Transfer", date);
                    ownedAccounts[m1 - 1] += newTransaction1;
                    Transaction newTransaction2(amount, "Transfer", date);
                    ownedSavingsAccounts[m2 - 1] += newTransaction2;

                } else {
                    cout << "Invalid\n";
                    return;
                }

            } else if ((n1 == 2 && src_case2) || (n1 == 2 && n1 == maxOption)) {
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < ownedSavingsAccounts.size(); i++) {
                    cout << i + 1 << ". " << ownedSavingsAccounts[i].getAccountNumber() << endl;
                }

                cout << "Choose: ";
                int m1; cin >> m1;
                if (m1 < 1 || m1 > ownedSavingsAccounts.size()) {
                    cout << "Invalid\n";
                    return;
                }
                cout << endl;

                cout << "Choose type of destination account:\n";
                int maxDestOption = 1;
                bool dest_case2 = false;

                vector<SavingsAccount> savingsDraft = ownedSavingsAccounts;
                savingsDraft.erase(savingsDraft.begin() + m1 - 1);

                if (savingsDraft.size() > 0) {
                    cout << "1. Savings\n";
                    if (ownedAccounts.size() > 0) {
                        cout << "2. Regular\n";
                        maxDestOption = 2;
                    }
                } else if (ownedAccounts.size() > 0) {
                    dest_case2 = true;
                    cout << "1. Regular\n";
                } else {
                    cout << "No destination accounts are available!\n";
                    return;
                }

                cout << "Choose: ";
                int n2; cin >> n2;
                if (dest_case2) n2++;
                cout << endl;

                if (n2 == 1) { 
                    cout << "Choose one of account numbers below:\n";
                    int order = 0;
                    vector<int> idxDes;
                    for (int i = 0; i < ownedSavingsAccounts.size(); i++) {
                        if (i != m1 - 1) {
                            order++;
                            idxDes.push_back(i);
                            cout << order << ". " << ownedSavingsAccounts[i].getAccountNumber() << endl;
                        }
                    }

                    cout << "Choose: ";
                    int m2; cin >> m2;
                    if (m2 < 1 || m2 > idxDes.size()) {
                        cout << "Invalid\n";
                        return;
                    }
                    int desIdx = idxDes[m2 - 1];

                    cout << "Enter amount of money you want to transfer: ";
                    double amount; cin >> amount;
                    if (amount <= 0) {
                        cout << "Invalid\n";
                        return;
                    } else if (amount > ownedSavingsAccounts[m1 - 1].getBalance()) {
                        cout << "Insufficient balance!";
                        return;
                    }
                    cout << "Transfer sucessful!\n\n";

                    ownedSavingsAccounts[m1 - 1].setBalance(ownedSavingsAccounts[m1 - 1].getBalance() - amount);
                    ownedSavingsAccounts[desIdx].setBalance(ownedSavingsAccounts[desIdx].getBalance() + amount);
                    ownedSavingsAccounts[desIdx].balanceInquiry();

                    Transaction t1(-amount, "Transfer", date);
                    ownedSavingsAccounts[m1 - 1] += t1;
                    Transaction t2(amount, "Transfer", date);
                    ownedSavingsAccounts[desIdx] += t2;

                } else if ((n2 == 2 && dest_case2) || (n2 == 2 && n2 == maxDestOption)) { 
                    cout << "Choose one of account numbers below:\n";
                    for (int i = 0; i < ownedAccounts.size(); i++) {
                        cout << i + 1 << ". " << ownedAccounts[i].getAccountNumber() << endl;
                    }
                    cout << "Choose: ";
                    int m2; cin >> m2;
                    if (m2 < 1 || m2 > ownedAccounts.size()) {
                        cout << "Invalid\n";
                        return;
                    }

                    cout << "Enter amount of money you want to transfer: ";
                    double amount; cin >> amount;
                    if (amount <= 0) {
                        cout << "Invalid\n";
                        return;
                    } else if (amount > ownedSavingsAccounts[m1 - 1].getBalance()) {
                        cout << "Insufficient balance!";
                        return;
                    }
                    cout << "Transfer sucessful!\n\n";

                    ownedSavingsAccounts[m1 - 1].setBalance(ownedSavingsAccounts[m1 - 1].getBalance() - amount);
                    ownedAccounts[m2 - 1].setBalance(ownedAccounts[m2 - 1].getBalance() + amount);
                    ownedAccounts[m2 - 1].balanceInquiry();

                    Transaction t1(-amount, "Transfer", date);
                    ownedSavingsAccounts[m1 - 1] += t1;
                    Transaction t2(amount, "Transfer", date);
                    ownedAccounts[m2 - 1] += t2;

                } else {
                    cout << "Invalid\n";
                    return;
                }
            } else cout << "Invalid\n"; 
        } 
};
int main(){
    // Create transaction history for regular accounts
    vector<Transaction> accHistory1 = {
        Transaction(200000, "Deposit", "01/09/2025"),
        Transaction(-50000, "Withdraw", "05/09/2025")
    };

    vector<Transaction> accHistory2 = {
        Transaction(100000, "Deposit", "02/09/2025"),
        Transaction(-30000, "Withdraw", "06/09/2025")
    };

    // Create transaction history for savings accounts
    vector<Transaction> savHistory1 = {
        Transaction(500000, "Deposit", "03/09/2025"),
        Transaction(10000, "Transfer", "10/09/2025")
    };

    vector<Transaction> savHistory2 = {
        Transaction(700000, "Deposit", "04/09/2025"),
        Transaction(12000, "Withdraw", "11/09/2025")
    };

    // Initialize regular accounts with balances and transaction histories
    Account acc1("ACC001", 150000, "Nguyen Khanh Hung", accHistory1);
    Account acc2("ACC002", 70000, "Nguyen Khanh Hung", accHistory2);

    // Initialize savings accounts with interest rates and transaction histories
    SavingsAccount sav1("SAV001", 510000, "Nguyen Khanh Hung", 5.0, savHistory1);
    SavingsAccount sav2("SAV002", 712000, "Nguyen Khanh Hung", 4.5, savHistory2);

    // Store the accounts in vectors
    vector<Account> accounts = {acc1, acc2};
    vector<SavingsAccount> savings = {sav1, sav2};

    // Create a customer with personal information and lists of accounts
    Customer customer("Nguyen Khanh Hung", "C001", accounts, savings);

    // Display customer information
    customer.displayInfo();
    cout << "\n";

    // Display a menu of functions for the user
    cout << "Choose function:\n";
    cout << "1. Open new account\n";
    cout << "2. Deposit\n";
    cout << "3. Withdraw\n";
    cout << "4. Transfer\n";
    cout << "5. Show total balances\n";
    cout << "6. Compare 2 accounts\n";
    cout << "Choose: ";
    int n; cin >> n;
    cout << "======================\n";

    // Handle the selected function
    switch (n) {
        case 1: {
            // Open a new account
            customer.openNewAccount();
            break;
        }

        case 2: {
            int maxOption = 1;
            bool src_case2 = false;
            if (customer.getOwnedAccounts().size() > 0){
                cout << "Choose type of account:\n";
                cout << "1. Regular\n";
                if (customer.getOwnedSavingsAccounts().size() > 0){
                    cout << "2. Savings\n";
                    maxOption = 2;
                }

            } else if (customer.getOwnedSavingsAccounts().size() > 0){
                bool src_case2 = true;
                cout << "Choose type of account:\n";
                cout << "1. Savings\n";
            }

            cout << "Choose: ";
            int m1; cin >> m1;
            if (src_case2) m1++;
            cout << endl;

            if (m1 == 1){
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k; cin >> k;
                if (k < 1 || k > customer.getOwnedAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                customer.getOwnedAccounts()[k - 1].deposit("16/9/2025");
            } else if ((m1 == 2 && src_case2) || (m1 == 2 && m1 == maxOption)) {
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k; cin >> k;
                if (k < 1 || k > customer.getOwnedSavingsAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                customer.getOwnedSavingsAccounts()[k - 1].deposit("16/9/2025");
            } else cout << "Invalid\n";
            break;
        }

        case 3: {
            int maxOption = 1;
            bool src_case2 = false;
            if (customer.getOwnedAccounts().size() > 0){
                cout << "Choose type of account:\n";
                cout << "1. Regular\n";
                if (customer.getOwnedSavingsAccounts().size() > 0){
                    cout << "2. Savings\n";
                    maxOption = 2;
                }

            } else if (customer.getOwnedSavingsAccounts().size() > 0){
                bool src_case2 = true;
                cout << "Choose type of account:\n";
                cout << "1. Savings\n";
            }

            cout << "Choose: ";
            int m1; cin >> m1;
            if (src_case2) m1++;
            cout << endl;

            if (m1 == 1){
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k; cin >> k;
                if (k < 1 || k > customer.getOwnedAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                customer.getOwnedAccounts()[k - 1].withdraw("16/9/2025");                
            } else if ((m1 == 2 && src_case2) || (m1 == 2 && m1 == maxOption)) {
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k; cin >> k;
                if (k < 1 || k > customer.getOwnedSavingsAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                customer.getOwnedSavingsAccounts()[k - 1].withdraw("16/9/2025");
            } else cout << "Invalid\n";
            break;
        }

        case 4: {
            // Transfer money between accounts
            customer.transfer("16/9/2025");
            break;
        }

        case 5: {
            // Calculate and display the total balance of all accounts
            customer.calculateTotalBalance();
            break;
        }
        case 6: {
            int maxOption = 1;
            bool src_case2 = false;
            if (customer.getOwnedAccounts().size() > 0){
                cout << "Choose type of source account:\n";
                cout << "1. Regular\n";
                if (customer.getOwnedSavingsAccounts().size() > 0){
                    cout << "2. Savings\n";
                    maxOption = 2;
                }

            } else if (customer.getOwnedSavingsAccounts().size() > 0){
                bool src_case2 = true;
                cout << "Choose type of source account:\n";
                cout << "1. Savings\n";
            }

            cout << "Choose: ";
            int m1; cin >> m1;
            if (src_case2) m1++;
            cout << endl;

            if (m1 == 1){
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k1; cin >> k1;
                if (k1 < 1 || k1 > customer.getOwnedAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                cout << "Choose type of destination account:\n";
                int maxDestOption = 1;
                bool dest_case2 = false;

                vector<Account> ownedAccounts_draft = customer.getOwnedAccounts();
                ownedAccounts_draft.erase(ownedAccounts_draft.begin() + k1 - 1);

                if (ownedAccounts_draft.size() > 0) {
                    cout << "1. Regular\n";
                    if (customer.getOwnedSavingsAccounts().size() > 0) {
                        cout << "2. Savings\n";
                        maxDestOption = 2;
                    }
                } else if (customer.getOwnedSavingsAccounts().size() > 0) {
                    dest_case2 = true;
                    cout << "1. Savings\n";
                } else {
                    cout << "No destination accounts are available!\n";
                    return 0;
                }

                cout << "Choose: ";
                int m2;
                cin >> m2;
                if (dest_case2) m2++;   
                cout << endl;

                if (m2 == 1) { 
                    cout << "Choose one of account numbers below:\n";
                    int order = 0;
                    vector<int> idxDes;
                    for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++) {
                        if (i != m1 - 1) {
                            order++;
                            idxDes.push_back(i);
                            cout << order << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                        }
                    }

                    cout << "Choose: ";
                    int k2; cin >> k2;
                    if (k2 < 1 || k2 > idxDes.size()) {
                        cout << "Invalid\n";
                        return 0;
                    }
                    int desIdx = idxDes[k2 - 1];

                    customer.getOwnedAccounts()[k1 - 1].compareAccount(customer.getOwnedAccounts()[desIdx]);
                } else if ((m2 == 2 && dest_case2) || (m2 == 2 && m2 == maxDestOption)){
                    cout << "Choose one of account numbers below:\n";
                    for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++){
                        cout << i + 1 << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                    }
                    
                    cout << "Choose: ";
                    int k2; cin >> k2;
                    if (k2 < 1 || k2 > customer.getOwnedSavingsAccounts().size()){
                        cout << "Invalid\n";
                        return 0;
                    }
                    cout << endl;

                    customer.getOwnedAccounts()[k1 - 1].compareAccount(customer.getOwnedSavingsAccounts()[k2 - 1]);
                }
            } else if ((m1 == 2 && src_case2) || (m1 == 2 && m1 == maxOption)) {
                cout << "Choose one of account numbers below:\n";
                for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++){
                    cout << i + 1 << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                }
                    
                cout << "Choose: ";
                int k1; cin >> k1;
                if (k1 < 1 || k1 > customer.getOwnedSavingsAccounts().size()){
                    cout << "Invalid\n";
                    return 0;
                }
                cout << endl;

                cout << "Choose type of destination account:\n";
                int maxDestOption = 1;
                bool dest_case2 = false;

                vector<SavingsAccount> savingsDraft = customer.getOwnedSavingsAccounts();
                savingsDraft.erase(savingsDraft.begin() + m1 - 1);

                if (savingsDraft.size() > 0) {
                    cout << "1. Savings\n";
                    if (customer.getOwnedAccounts().size() > 0) {
                        cout << "2. Regular\n";
                        maxDestOption = 2;
                    }
                } else if (customer.getOwnedAccounts().size() > 0) {
                    dest_case2 = true;
                    cout << "1. Regular\n";
                } else {
                    cout << "No destination accounts are available!\n";
                    return 0;
                }

                cout << "Choose: ";
                int m2; cin >> m2;
                if (dest_case2) m2++;
                cout << endl;

                if (m2 == 1) { 
                    cout << "Choose one of account numbers below:\n";
                    int order = 0;
                    vector<int> idxDes;
                    for (int i = 0; i < customer.getOwnedSavingsAccounts().size(); i++) {
                        if (i != m1 - 1) {
                            order++;
                            idxDes.push_back(i);
                            cout << order << ". " << customer.getOwnedSavingsAccounts()[i].getAccountNumber() << endl;
                        }
                    }

                    cout << "Choose: ";
                    int k2; cin >> k2;
                    if (k2 < 1 || k2 > idxDes.size()) {
                        cout << "Invalid\n";
                        return 0;
                    }
                    int desIdx = idxDes[m2 - 1];

                    customer.getOwnedSavingsAccounts()[k1 - 1].compareAccount(customer.getOwnedSavingsAccounts()[desIdx]);
                } else if ((m2 == 2 && dest_case2) || (m2 == 2 && m2 == maxDestOption)) { 
                    cout << "Choose one of account numbers below:\n";
                    for (int i = 0; i < customer.getOwnedAccounts().size(); i++) {
                        cout << i + 1 << ". " << customer.getOwnedAccounts()[i].getAccountNumber() << endl;
                    }
                    cout << "Choose: ";
                    int k2; cin >> k2;
                    if (k2 < 1 || k2 > customer.getOwnedAccounts().size()) {
                        cout << "Invalid\n";
                        return 0;
                    }

                    customer.getOwnedSavingsAccounts()[k1 - 1].compareAccount(customer.getOwnedAccounts()[k2 - 1]);
                } else cout << "Invalid\n"; 
            } else cout << "Invalid\n";
            break;
        }
        default: {
            // Entered an invalid choice
            cout << "Invalid\n";
            break;
        }
    }
}