#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

const int MAX_ITEMS = 20;
const int MAX_STAFF = 30;
const int MAX_ORDER_ITEMS = 30;
const int MAX_TABLES = 10;

class Item {
public:
    string name;
    double price;
    string category;

    Item(const string& n = "Invalid", double p = 0.0, const string& cat = "General") : name(n), price(p), category(cat) {}
};

class Menu {
private:
    Item items[MAX_ITEMS];
    int itemCount;

public:
    Menu() : itemCount(0) {}

    void addItem(const string& name, double price, const string& category) {
        if (itemCount < MAX_ITEMS) {
            items[itemCount] = Item(name, price, category);
            itemCount++;
        }
    }

    void displayMenu() {
        cout << "\n--- MENU ---\n";
        string categories[] = {"Drinks", "Snacks", "Main Course"};
        for (int i = 0; i < 3; i++) {
            cout << "\nCategory: " << categories[i] << endl;
            for (int j = 0; j < itemCount; j++) {
                if (items[j].category == categories[i]) {
                    cout << j + 1 << ". " << items[j].name << " - $" << items[j].price << endl;
                }
            }
        }
    }

    Item getItem(int index) {
        if (index >= 0 && index < itemCount) {
            return items[index];
        }
        return Item();
    }

    void loadFromFile(const string& filename = "men.txt") {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            size_t pos1 = line.find(',');
            size_t pos2 = line.rfind(',');

            if (pos1 != string::npos && pos2 != string::npos && pos1 != pos2) {
                string name = line.substr(0, pos1);
                double price = stod(line.substr(pos1 + 1, pos2 - pos1 - 1));
                string category = line.substr(pos2 + 1);
                addItem(name, price, category);
            }
        }
        file.close();
    }

    void saveToFile(const string& filename = "men.txt") {
        ofstream file(filename);
        for (int i = 0; i < itemCount; i++) {
            file << items[i].name << "," << items[i].price << "," << items[i].category << endl;
        }
        file.close();
    }
};

class TableReservation {
private:
    string reservedTables[MAX_TABLES];

public:
    TableReservation() {
        for (int i = 0; i < MAX_TABLES; i++) {
            reservedTables[i] = "";
        }
    }

    void reserveTable(int tableNumber, const string& customerName) {
        if (tableNumber > 0 && tableNumber <= MAX_TABLES) {
            if (reservedTables[tableNumber - 1] == "") {
                reservedTables[tableNumber - 1] = customerName;
                cout << "Table " << tableNumber << " reserved for " << customerName << endl;
            } else {
                cout << "Table " << tableNumber << " is already reserved.\n";
            }
        } else {
            cout << "Invalid table number.\n";
        }
    }

    void viewReservations() {
        bool found = false;
        for (int i = 0; i < MAX_TABLES; i++) {
            if (reservedTables[i] != "") {
                cout << "Table " << i + 1 << ": " << reservedTables[i] << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "No reservations found.\n";
        }
    }

    void saveToFile(const string& filename = "Reservations.txt") {
        ofstream file(filename);
        for (int i = 0; i < MAX_TABLES; i++) {
            if (reservedTables[i] != "") {
                file << "Table " << i + 1 << ": " << reservedTables[i] << endl;
            }
        }
        file.close();
    }

    void loadFromFile(const string& filename = "Reservations.txt") {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            int tableNumber;
            size_t pos = line.find(":");
            if (pos != string::npos) {
                tableNumber = stoi(line.substr(5, pos - 5));
                string customerName = line.substr(pos + 2);
                reservedTables[tableNumber - 1] = customerName;
            }
        }
        file.close();
    }
};

class SalesTracker {
private:
    string soldItems[MAX_ITEMS];
    int soldQuantities[MAX_ITEMS];
    double soldPrices[MAX_ITEMS];
    int soldCount;

public:
    SalesTracker() : soldCount(0) {}

    void recordSale(const string& itemName, double price, int quantity) {
        for (int i = 0; i < soldCount; i++) {
            if (soldItems[i] == itemName) {
                soldQuantities[i] += quantity;
                return;
            }
        }
        if (soldCount < MAX_ITEMS) {
            soldItems[soldCount] = itemName;
            soldQuantities[soldCount] = quantity;
            soldPrices[soldCount] = price;
            soldCount++;
        }
    }

    void generateReport(const string& filename = "Daily_Report.txt") {
        ofstream file(filename);
        double totalRevenue = 0.0, totalCost = 0.0;
        file << "--- DAILY SALES REPORT ---\n\n";
        file << "Item\t\tQuantity Sold\tRevenue\n";
        file << "-----------------------------------------\n";

        for (int i = 0; i < soldCount; i++) {
            double itemRevenue = soldPrices[i] * soldQuantities[i];
            double itemCost = (soldPrices[i] * 0.7) * soldQuantities[i];
            file << soldItems[i] << "\t\t" << soldQuantities[i] << "\t\t$" << itemRevenue << "\n";
            totalRevenue += itemRevenue;
            totalCost += itemCost;
        }

        double netProfit = totalRevenue - totalCost;
        file << "\nTotal Revenue: $" << totalRevenue;
        file << "\nTotal Cost:    $" << totalCost;
        file << "\nNet Profit:    $" << netProfit << "\n";

        file.close();
        cout << "Daily report generated: " << filename << endl;
    }
};

class Order {
private:
    Item orderItems[MAX_ORDER_ITEMS];
    int orderQty[MAX_ORDER_ITEMS];
    int orderCount;
    double total;
    bool discountApplied;
    SalesTracker* tracker;

public:
    Order(SalesTracker* t = nullptr) : orderCount(0), total(0.0), discountApplied(false), tracker(t) {}

    void addItem(const Item& item, int qty) {
        if (orderCount < MAX_ORDER_ITEMS) {
            orderItems[orderCount] = item;
            orderQty[orderCount] = qty;
            total += item.price * qty;
            orderCount++;
            if (tracker != nullptr) {
                tracker->recordSale(item.name, item.price, qty);
            }
        }
    }

    double calculateTotal() {
        double finalTotal = total;
        if (total > 50 && !discountApplied) {
            finalTotal -= total * 0.10;  // 10% discount for bills above $50
            discountApplied = true;
        }
        return finalTotal;
    }

    void printBill(const string& customerName) {
        for (int i = 0; i < orderCount; i++) {
            cout << orderItems[i].name << " x" << orderQty[i] << " = $" << orderItems[i].price * orderQty[i] << endl;
        }

        double finalAmount = calculateTotal();
        cout << "Total: $" << finalAmount << endl;
        if (discountApplied) {
            cout << "Discount applied: 10%\n";
        }

        string filename = "Receipt_" + customerName + ".txt";
        ofstream file(filename);
        file << "--- BILL ---\n";
        for (int i = 0; i < orderCount; i++) {
            file << orderItems[i].name << " x" << orderQty[i] << " = $" << orderItems[i].price * orderQty[i] << "\n";
        }
        file << "Total: $" << finalAmount << endl;
        if (discountApplied) {
            file << "Discount applied: 10%\n";
        }
        file.close();
        cout << "Receipt saved to " << filename << endl;
    }
};

int main() {
    Menu menu;
    menu.loadFromFile();

    TableReservation tableReservation;
    tableReservation.loadFromFile();

    SalesTracker tracker;

    char choice;
    while (true) {
        cout << "\n1. View Menu\n";
        cout << "2. Place Order\n";
        cout << "3. Reserve Table\n";
        cout << "4. View Reservations\n";
        cout << "5. Exit\n";
        cout << "Select an option: ";
        cin >> choice;
        cin.ignore();

        if (choice == '1') {
            menu.displayMenu();
        }
        else if (choice == '2') {
            string customerName;
            cout << "Enter Customer Name: ";
            getline(cin, customerName);

            Order order(&tracker);
            char cont = 'y';
            while (cont == 'y' || cont == 'Y') {
                menu.displayMenu();
                int itemNo, qty;
                cout << "Enter item number: ";
                cin >> itemNo;
                cout << "Enter quantity: ";
                cin >> qty;
                cin.ignore();
                Item selected = menu.getItem(itemNo - 1);
                order.addItem(selected, qty);
                cout << "Add more items? (y/n): ";
                cin >> cont;
                cin.ignore();
            }
            order.printBill(customerName);
        }
        else if (choice == '3') {
            int tableNumber;
            string customerName;
            cout << "Enter customer name: ";
            cin.ignore();
            getline(cin, customerName);
            cout << "Enter table number (1-" << MAX_TABLES << "): ";
            cin >> tableNumber;
            tableReservation.reserveTable(tableNumber, customerName);
        }
        else if (choice == '4') {
            tableReservation.viewReservations();
        }
        else if (choice == '5') {
            tableReservation.saveToFile();
            char generateReport;
            cout << "Do you want to generate today's report? (y/n): ";
            cin >> generateReport;
            if (generateReport == 'y' || generateReport == 'Y') {
                tracker.generateReport();
            }
            cout << "Exiting system...\n";
            break;
        }
    }

    return 0;
}
