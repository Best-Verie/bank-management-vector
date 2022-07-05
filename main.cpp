//
// Created by verie on 7/4/2022.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <cmath>
using namespace std;

class Account
{
public:
    int accountId;
    int accountNumber;
    string username;
    int balance;
};

bool is_empty_file(std::ifstream &pFile)
{
    return pFile.peek() == std::ifstream::traits_type::eof();
}

vector<Account> getAllAccounts()
{
    ifstream file("account.txt", ios::in);
    vector<Account> accounts;
    string line;
    while (getline(file, line))
    {
        stringstream stringstream1(line);
        vector<string> fields;
        string field;
        while (getline(stringstream1, field, ','))
        {
            fields.push_back(field);
        }

        Account account;
        account.accountId = stoi(fields[0]);
        account.accountNumber = stoi(fields[1]);
        account.username = fields[2];
        account.balance = stoi(fields[3]);

        accounts.push_back(account);
    }

    file.close();
    return accounts;
}

int getLatestId()
{
    vector<Account> accounts = getAllAccounts();
    return accounts.back().accountId;
}

bool accountNumberExists(int accNo)
{
    vector<Account> accounts = getAllAccounts();
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountNumber == accNo)
        {
            return true;
        }
    }
    return false;
}

int generateAccountNumber()
{
start:
    int number = rand() % 9000000 + 1000000;
    if (accountNumberExists(number))
    {
        goto start;
    }
    return number;
}
void save_account()
{
    Account account;
    ofstream account_file("account.txt", ios::out | ios::app);

    cout << "Enter username: ";
    cin >> account.username;
    account.accountId = getLatestId() + 1;
    account.accountNumber = generateAccountNumber();
    account.balance = 0;
    account_file << account.accountId << "," << account.accountNumber << "," << account.username << "," << account.balance << endl;

    cout << "Account created successfully!" << endl;
    cout << "Account ID: " << account.accountId << endl;
    cout << "Account Number: " << account.accountNumber << endl;
    cout << "Balance: " << account.balance << endl;
    account_file.close();
}

void update_to_file_from_vector(vector<Account> accounts)
{
    ofstream temp_file("temp.txt", ios::out | ios::app);
    for (int i = 0; i < accounts.size(); i++)
    {
        Account account = accounts[i];
        temp_file << account.accountId << "," << account.accountNumber << "," << account.username << "," << account.balance << endl;
    }
    temp_file.close();
    remove("account.txt");
    rename("temp.txt", "account.txt");
}

void update_account(int acno)
{
    vector<Account> accounts = getAllAccounts();
    for (int i = 0; i < accounts.size(); i++)
    {
        Account account = accounts[i];
        if (account.accountNumber == acno)
        {
            cout << "Enter updated username: ";
            cin >> accounts[i].username;
        }
    }
    update_to_file_from_vector(accounts);
}

void print_accounts(vector<Account> accounts)
{
    for (int i = 0; i < accounts.size(); i++)
    {
        cout << accounts[i].accountId << " " << accounts[i].accountNumber << " " << accounts[i].username << " " << accounts[i].balance << endl;
    }
}

void sorting_accounts_by_input()
{
    vector<Account> accounts = getAllAccounts();

    for (int step = 0; step < (accounts.size() - 1); ++step)
    {

        int swapped = 0;

        for (int i = 0; i < (accounts.size()); ++i)
        {

            if (accounts[i].balance < accounts[i + 1].balance)
            {

                Account temp = accounts[i];
                accounts[i] = accounts[i + 1];
                accounts[i + 1] = temp;

                swapped = 1;
            }
        }
        if (swapped == 0)
            break;
    }

    print_accounts(accounts);
}

void deleteAccount(int acNo)
{

    vector<Account> accounts = getAllAccounts();
    for (int i = 0; i < accounts.size(); i++)
    {
        Account account = accounts[i];
        if (account.accountNumber == acNo)
        {
            accounts.erase(accounts.begin() + i);
        }
    }
    update_to_file_from_vector(accounts);
}

int increase_balance(int account_number, int amount)
{

    int new_balance = 0;

    vector<Account> accounts = getAllAccounts();

    for (int i = 0; i < accounts.size(); i++)
    {
        Account account = accounts[i];

        if (account.accountNumber == account_number)
        {

            accounts[i].balance += amount;
            new_balance = accounts[i].balance;
        }
    }

    update_to_file_from_vector(accounts);

    return new_balance;
}

int decrease_balance(int account_number, int amount)
{

    int new_balance = 0;

    vector<Account> accounts = getAllAccounts();

    for (int i = 0; i < accounts.size(); i++)
    {
        Account account = accounts[i];

        if (account.accountNumber == account_number)
        {

            accounts[i].balance -= amount;
            new_balance = accounts[i].balance;
        }
    }

    update_to_file_from_vector(accounts);

    return new_balance;
}

void displaySingle(int accNo)
{
    vector<Account> accounts = getAllAccounts();
    for (int i = 0; i < accounts.size(); i++)
    {
        if (accounts[i].accountId == accNo)
        {
            cout << "\n\n\t " << accounts[i].accountId << " " << accounts[i].accountNumber << " " << accounts[i].username << " " << accounts[i].balance << " " << endl;
        }
    }
    cout << "\n\n\t ACCOUNT NOT FOUND!";
}

int deposit_to_account()
{
    int account_number, amount;
    cout << "Enter account number : " << endl;
    cin >> account_number;

    if (!accountNumberExists(account_number))
    {
        cout << "Sorry, your account is not found" << endl;
        deposit_to_account();
    }

    cout << "Enter amount to deposit : " << endl;
    cin >> amount;

    int new_balance = increase_balance(account_number, amount);

    cout << "Deposited: " << amount << ", " << new_balance << endl;

    return new_balance;
}

int withdraw_to_account()
{
    int account_number, amount;
    cout << "Enter account number : " << endl;
    cin >> account_number;

    if (!accountNumberExists(account_number))
    {
        cout << "Sorry, your account is not found" << endl;
        deposit_to_account();
    }

    cout << "Enter amount to withdraw : " << endl;
    cin >> amount;

    if (amount <= 10000)
    {
        cout << "Sorry, can't withdraw this amount!" << endl;
    }
    int new_balance = decrease_balance(account_number, amount);

    cout << "withdrew: " << amount << ", " << new_balance << endl;

    return new_balance;
}
void intro()
{
    cout << "\n\t BANK";
    cout << "\n\t MANAGEMENT";
    cout << "\n\t SYSTEM";
    cout << "\n\t MADE BY: verie Best";
    cout << "\n\n\t press any key to continue";
    cin.get();
}
int main()
{
    int ch;
    int accountNumber;
    intro();
    do
    {
        cout << "\n\n\n\tMAIN MENU";
        cout << "\n\n\t01. NEW ACCOUNT";
        cout << "\n\n\t02. DEPOSIT AMOUNT";
        cout << "\n\n\t03. WITHDRAW AMOUNT";
        cout << "\n\n\t04. BALANCE ENQUIRY";
        cout << "\n\n\t05. SORT ACCOUNT HOLDER LIST";
        cout << "\n\n\t06. CLOSE AN ACCOUNT";
        cout << "\n\n\t07. MODIFY AN ACCOUNT";
        cout << "\n\n\t08. EXIT";
        cout << "\n\n\tSelect Your Option (1-8) ";
        cin >> ch;
        //        system("cls");
        switch (ch)
        {

        case 1:
            save_account();
            break;

        case 2:
        {
            int total_balance = deposit_to_account();
            cout << "Total balance: " << total_balance << endl;
            break;
        }

        case 3:
        {
            int total_balance = withdraw_to_account();
            cout << "Total balance: " << total_balance << endl;
            break;
        }
        case 4:
        {

            int account_number;
        singleAccount:
            cout << "Enter account number : ";
            cin >> account_number;
            if (!accountNumberExists(account_number))
                goto singleAccount;
            displaySingle(account_number);
            break;
        }

        case 5:
            sorting_accounts_by_input();

        case 7:
        {
            int account_number;
        account:
            cout << "Enter account number : ";
            cin >> account_number;
            if (!accountNumberExists(account_number))
                goto account;
            update_account(account_number);
        }
        break;

        case 6:
        {
            int account_number;
        update_account:
            cout << "Enter account number : ";
            cin >> account_number;
            cout << "fghj" << endl;

            if (!accountNumberExists(account_number))
                goto update_account;

            deleteAccount(account_number);
        }

        default:
            cout << "Invalid option!" << endl;
            break;
        }
    } while (ch != 8);

    return 0;
}