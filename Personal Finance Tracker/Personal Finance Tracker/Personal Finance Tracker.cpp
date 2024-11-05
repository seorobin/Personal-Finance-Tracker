#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
#include <sstream>

using namespace std;

class Expense {
private:
    double amount;
    string category;
    string description;
    string date;

public:
    Expense(double amt, string cat, string desc, string date)
        : amount(amt), category(cat), description(desc), date(date) {}

    double getAmount() const { return amount; }
    string getCategory() const { return category; }
    string getDescription() const { return description; }
    string getDate() const { return date; }

    // display expense details
    void display() const {
        cout << left << setw(15) << date << setw(15) << category << setw(15) << description
            << "$" << setw(10) << fixed << setprecision(2) << amount << endl;
    }

    // Function to export expense to CSV format
    string toCSV() const {
        return date + "," + category + "," + description + "," + to_string(amount);
    }
};

class FinanceTracker {
private:
    vector<Expense> expenses;  

public:
   
    void addExpense() {
        double amount;
        string category, description, date;

        cout << "Enter amount: $";
        cin >> amount;
        cin.ignore();  // To ignore the newline character left in the input buffer

        cout << "Enter category (e.g., Food, Transport, Entertainment): ";
        getline(cin, category);

        cout << "Enter description: ";
        getline(cin, description);

        cout << "Enter date (YYYY-MM-DD): ";
        getline(cin, date);

        Expense newExpense(amount, category, description, date);
        expenses.push_back(newExpense);
        cout << "Expense added successfully!" << endl;
    }

  
    void displayExpenses() const {
        cout << "\n-----------------------------------------------------\n";
        cout << left << setw(15) << "Date" << setw(15) << "Category" << setw(15) << "Description"
            << setw(15) << "Amount" << endl;
        cout << "-----------------------------------------------------\n";

        for (const auto& expense : expenses) {
            expense.display();
        }

        cout << "-----------------------------------------------------\n";
    }

    // monthly
    void generateMonthlyReport(const string& month) const {
        double totalAmount = 0;
        map<string, double> categorySummary;

        for (const auto& expense : expenses) {
            if (expense.getDate().substr(5, 2) == month) {  // Check if the month matches
                totalAmount += expense.getAmount();
                categorySummary[expense.getCategory()] += expense.getAmount();
            }
        }

        cout << "\nMonthly Report for Month: " << month << endl;
        cout << "-----------------------------------------------------\n";
        cout << left << setw(20) << "Category" << setw(15) << "Amount" << endl;
        cout << "-----------------------------------------------------\n";

        for (const auto& entry : categorySummary) {
            cout << left << setw(20) << entry.first << "$" << setw(15) << fixed << setprecision(2) << entry.second << endl;
        }

        cout << "-----------------------------------------------------\n";
        cout << "Total Expense: $" << fixed << setprecision(2) << totalAmount << endl;
    }

    // annual report
    void generateAnnualReport() const {
        double totalAmount = 0;
        map<string, double> categorySummary;

        for (const auto& expense : expenses) {
            totalAmount += expense.getAmount();
            categorySummary[expense.getCategory()] += expense.getAmount();
        }

        cout << "\nAnnual Report" << endl;
        cout << "-----------------------------------------------------\n";
        cout << left << setw(20) << "Category" << setw(15) << "Amount" << endl;
        cout << "-----------------------------------------------------\n";

        for (const auto& entry : categorySummary) {
            cout << left << setw(20) << entry.first << "$" << setw(15) << fixed << setprecision(2) << entry.second << endl;
        }

        cout << "-----------------------------------------------------\n";
        cout << "Total Annual Expense: $" << fixed << setprecision(2) << totalAmount << endl;
    }

    // CSV file
    void saveToFile(const string& filename) const {
        ofstream file(filename);
        if (file.is_open()) {
            file << "Date,Category,Description,Amount\n";  // Header
            for (const auto& expense : expenses) {
                file << expense.toCSV() << "\n";
            }
            file.close();
            cout << "Expenses saved to file successfully!" << endl;
        }
        else {
            cout << "Error saving to file." << endl;
        }
    }

    // load expenses from a CSV file
    void loadFromFile(const string& filename) {
        ifstream file(filename);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string date, category, description;
                double amount;

                getline(ss, date, ',');
                getline(ss, category, ',');
                getline(ss, description, ',');
                ss >> amount;

                if (!date.empty() && !category.empty()) {
                    Expense expense(amount, category, description, date);
                    expenses.push_back(expense);
                }
            }
            file.close();
            cout << "Expenses loaded from file successfully!" << endl;
        }
        else {
            cout << "Error opening file!" << endl;
        }
    }
};

int main() {
    FinanceTracker tracker;
    int choice;

    tracker.loadFromFile("expenses.csv");

    while (true) {
        cout << "\n--- Personal Finance Tracker ---\n";
        cout << "1. Add Expense\n";
        cout << "2. View Expenses\n";
        cout << "3. Monthly Report\n";
        cout << "4. Annual Report\n";
        cout << "5. Save to CSV\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(); // To ignore the newline character left in the input buffer

        switch (choice) {
        case 1:
            tracker.addExpense();
            break;
        case 2:
            tracker.displayExpenses();
            break;
        case 3: {
            string month;
            cout << "Enter month (MM format): ";
            cin >> month;
            tracker.generateMonthlyReport(month);
            break;
        }
        case 4:
            tracker.generateAnnualReport();
            break;
        case 5:
            tracker.saveToFile("expenses.csv");
            break;
        case 6:
            cout << "Goodbye!" << endl;
            return 0;
        default:
            cout << "Invalid choice. Try again!" << endl;
        }
    }

    return 0;
}
