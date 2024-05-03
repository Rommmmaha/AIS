#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <ctime>
#include <iomanip>
#include <map>
#include <algorithm>
using namespace std;
struct Deposit {
    int id;
    string type;
    double amount;
    double rate;
    double inflation_rate;
    time_t open_date;
    time_t close_date;
};
vector<Deposit> readFromFile(const string& filename) {
    vector<Deposit> deposits;
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            istringstream iss(line);
            Deposit deposit;
            iss >> deposit.id >> deposit.type >> deposit.amount >> deposit.rate >> deposit.inflation_rate;
            iss >> deposit.open_date >> deposit.close_date;
            deposits.push_back(deposit);
        }
        file.close();
    }
    return deposits;
}
void writeToFile(const string& filename, const vector<Deposit>& deposits) {
    ofstream file(filename);
    if (file.is_open()) {
        for (const auto& deposit : deposits) {
            file << deposit.id << " " << deposit.type << " " << deposit.amount << " " << deposit.rate << " " << deposit.inflation_rate << " " << deposit.open_date << " " << deposit.close_date << "\n";
        }
        file.close();
    }
}
void createDeposit(vector<Deposit>& deposits) {
    Deposit deposit;
    deposit.id = deposits.empty() ? 1 : deposits.back().id + 1;
    cout << "Enter deposit type (term, cumulative, savings): ";
    cin >> deposit.type;
    cout << "Enter deposit amount: ";
    cin >> deposit.amount;
    cout << "Enter deposit rate: ";
    cin >> deposit.rate;
    cout << "Enter inflation rate: ";
    cin >> deposit.inflation_rate;
    deposit.open_date = time(nullptr);
    deposit.close_date = 0;
    deposits.push_back(deposit);
    cout << "Deposit created successfully! ID: " << deposit.id << "\n";
}
void depositAmount(vector<Deposit>& deposits) {
    int id;
    cout << "Enter deposit ID: ";
    cin >> id;
    auto it = find_if(deposits.begin(), deposits.end(), [id](const Deposit& deposit) { return deposit.id == id; });
    if (it != deposits.end()) {
        double amount;
        cout << "Enter amount to deposit: ";
        cin >> amount;
        it->amount += amount;
        cout << "Deposit amount updated successfully!\n";
    }
    else {
        cout << "Deposit not found!\n";
    }
}
void closeDeposit(vector<Deposit>& deposits) {
    int id;
    cout << "Enter deposit ID: ";
    cin >> id;
    auto it = find_if(deposits.begin(), deposits.end(), [id](const Deposit& deposit) { return deposit.id == id; });
    if (it != deposits.end()) {
        it->close_date = time(nullptr);
        cout << "Deposit closed successfully!\n";
    }
    else {
        cout << "Deposit not found!\n";
    }
}
void viewDeposits(const vector<Deposit>& deposits) {
    cout << left << setw(8) << "ID" << setw(16) << "Type" << setw(16) << "Amount" << setw(8) << "Rate" << setw(12) << "Inflation" << setw(20) << "Open Date" << setw(20) << "Close Date" << "\n";
    for (const auto& deposit : deposits) {
        cout << left << setw(8) << deposit.id << setw(16) << deposit.type << setw(16) << deposit.amount << setw(8) << deposit.rate << setw(12) << deposit.inflation_rate << setw(20) << ctime(&deposit.open_date) << setw(20) << (deposit.close_date ? ctime(&deposit.close_date) : "Open\n");
    }
}
void filterByOpenDate(const vector<Deposit>& deposits) {
    time_t start_date, end_date;
    cout << "Enter start date (YYYY-MM-DD): ";
    string start_str;
    cin >> start_str;
    tm start_tm = {};
    istringstream start_ss(start_str);
    start_ss >> get_time(&start_tm, "%Y-%m-%d");
    start_date = mktime(&start_tm);

    cout << "Enter end date (YYYY-MM-DD): ";
    string end_str;
    cin >> end_str;
    tm end_tm = {};
    istringstream end_ss(end_str);
    end_ss >> get_time(&end_tm, "%Y-%m-%d");
    end_date = mktime(&end_tm);

    cout << left << setw(8) << "ID" << setw(16) << "Type" << setw(16) << "Amount" << setw(8) << "Rate" << setw(12) << "Inflation" << setw(20) << "Open Date" << setw(20) << "Close Date" << "\n";
    for (const auto& deposit : deposits) {
        if (deposit.open_date >= start_date && deposit.open_date <= end_date) {
            cout << left << setw(8) << deposit.id << setw(16) << deposit.type << setw(16) << deposit.amount << setw(8) << deposit.rate << setw(12) << deposit.inflation_rate << setw(20) << ctime(&deposit.open_date) << setw(20) << (deposit.close_date ? ctime(&deposit.close_date) : "Open\n");
        }
    }
}
void showDepositDistribution(const vector<Deposit>& deposits) {
    map<string, double> deposit_totals;
    for (const auto& deposit : deposits) {
        deposit_totals[deposit.type] += deposit.amount;
    }

    cout << "Deposit Distribution:\n";
    for (const auto& entry : deposit_totals) {
        cout << entry.first << ": " << entry.second << "\n";
    }
}
int main() {
    vector<Deposit> deposits = readFromFile("db.txt");

    int choice;
    do {
        cout << "\nMenu:\n"
            << "1. Create Deposit\n"
            << "2. Deposit Amount\n"
            << "3. Close Deposit\n"
            << "4. View Deposits\n"
            << "5. Filter by Open Date\n"
            << "6. Show Deposit Distribution\n"
            << "0. Exit\n"
            << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            createDeposit(deposits);
            writeToFile("db.txt", deposits);
            break;
        case 2:
            depositAmount(deposits);
            writeToFile("db.txt", deposits);
            break;
        case 3:
            closeDeposit(deposits);
            writeToFile("db.txt", deposits);
            break;
        case 4:
            viewDeposits(deposits);
            break;
        case 5:
            filterByOpenDate(deposits);
            break;
        case 6:
            showDepositDistribution(deposits);
            break;
        case 0:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 0);

    return 0;
}