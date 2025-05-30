#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
using namespace std;

const int MAX_ITEMS = 20;
const int MAX_ORDER_ITEMS = 30;
const int MAX_TABLES = 10;

// ===================== ITEM CLASS =====================
class Item {
public:
    string name;
    double price;
    string category;

    Item(const string& n = "Invalid", double p = 0.0, const string& cat = "General") : name(n), price(p), category(cat) {}
};

// ===================== ABSTRACT MENU BASE =====================
class MenuBase {
public:
    virtual void addItem(const string&, double, const string&) = 0;
    virtual void displayMenu() = 0;
    virtual Item getItem(int index) = 0;
    virtual void loadFromFile(const string& filename) = 0;
    virtual void saveToFile(const string& filename) = 0;
    virtual ~MenuBase() {}
};

// ===================== MENU CLASS =====================
class Menu : public MenuBase {
private:
    Item items[MAX_ITEMS];
    int itemCount;

public:
    Menu() : itemCount(0) {}

    void addItem(const string& name, double price, const string& category) override {
        if (itemCount < MAX_ITEMS) {
            items[itemCount++] = Item(name, price, category);
        }
    }

    void displayMenu() override {
        cout << "\n--- MENU ---\n";
        string categories[] = { "Drinks", "Snacks", "Main Course" };
        for (const string& cat : categories) {
            cout << "\nCategory: " << cat << endl;
            for (int j = 0; j < itemCount; j++) {
                if (items[j].category == cat) {
                    cout << j + 1 << ". " << items[j].name << " - $" << items[j].price << endl;
                }
            }
        }
    }

    Item getItem(int index) override {
        if (index >= 0 && index < itemCount) {
            return items[index];
        }
        return Item();
    }

    void loadFromFile(const string& filename = "men.txt") override {
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

    void saveToFile(const string& filename = "men.txt") override {
        ofstream file(filename);
        for (int i = 0; i < itemCount; i++) {
            file << items[i].name << "," << items[i].price << "," << items[i].category << endl;
        }
        file.close();
    }
};

// ===================== ABSTRACT RESERVATION BASE =====================
class ReservationBase {
public:
    virtual void reserveTable(int tableNumber, const string& customerName) = 0;
    virtual void viewReservations() = 0;
    virtual void saveToFile(const string& filename) = 0;
    virtual void loadFromFile(const string& filename) = 0;
    virtual ~ReservationBase() {}
};

// ===================== TABLE RESERVATION CLASS =====================
class TableReservation : public ReservationBase {
private:
    string reservedTables[MAX_TABLES];

public:
    TableReservation() {
        for (int i = 0; i < MAX_TABLES; i++) reservedTables[i] = "";
    }

    void reserveTable(int tableNumber, const string& customerName) override {
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

    void viewReservations() override {
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

    void saveToFile(const string& filename = "Reservations.txt") override {
        ofstream file(filename);
        for (int i = 0; i < MAX_TABLES; i++) {
            if (reservedTables[i] != "") {
                file << "Table " << i + 1 << ": " << reservedTables[i] << endl;
            }
        }
        file.close();
    }

    void loadFromFile(const string& filename = "Reservations.txt") override {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(":");
            if (pos != string::npos) {
                int tableNumber = stoi(line.substr(6, pos - 6));
                string customerName = line.substr(pos + 2);
                reservedTables[tableNumber - 1] = customerName;
            }
        }
        file.close();
    }
};

// ===================== ABSTRACT SALES BASE =====================
class SalesBase {
public:
    virtual void recordSale(const string&, double, int) = 0;
    virtual void generateReport(const string& filename) = 0;
    virtual ~SalesBase() {}
};

// ===================== SALES TRACKER CLASS =====================
class SalesTracker : public SalesBase {
private:
    string soldItems[MAX_ITEMS];
    int soldQuantities[MAX_ITEMS];
    double soldPrices[MAX_ITEMS];
    int soldCount;

public:
    SalesTracker() : soldCount(0) {}

    void recordSale(const string& itemName, double price, int quantity) override {
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

    void generateReport(const string& filename = "Daily_Report.txt") override {
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

// ===================== ORDER CLASS =====================
class Order {
private:
    Item orderItems[MAX_ORDER_ITEMS];
    int orderQty[MAX_ORDER_ITEMS];
    int orderCount;
    double total;
    bool discountApplied;
    SalesBase* tracker;

public:
    Order(SalesBase* t = nullptr) : orderCount(0), total(0.0), discountApplied(false), tracker(t) {}

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
            finalTotal -= total * 0.10;
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
    MenuBase* menu = new Menu();
    menu->loadFromFile("men.txt");

    ReservationBase* reservationSystem = new TableReservation();
    reservationSystem->loadFromFile("Reservations.txt");

    SalesBase* sales = new SalesTracker();

    int choice;
    do {
        cout << "\n--- CAFE MANAGEMENT SYSTEM ---\n";
        
        cout << "0. Exit\n";
        cout << "1. View Menu\n";
        cout << "2. Reserve Table\n";
        cout << "3. View Reservations\n";
        cout << "4. Place Order\n";
        cout << "5. Generate Daily Report\n";
        
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                menu->displayMenu();
                break;

            case 2: {
                int tableNumber;
                string customerName;
                cout << "Enter table number (1-10): ";
                cin >> tableNumber;
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);
                reservationSystem->reserveTable(tableNumber, customerName);
                reservationSystem->saveToFile("Reservations.txt");
                break;
            }

            case 3:
                reservationSystem->viewReservations();
                break;

            case 4: {
                string customerName;
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);

                Order order(sales);
                char moreItems;
                do {
                    menu->displayMenu();
                    int itemIndex, quantity;
                    cout << "Enter item number: ";
                    cin >> itemIndex;
                    cout << "Enter quantity: ";
                    cin >> quantity;

                    Item item = menu->getItem(itemIndex - 1);
                    if (item.name != "Invalid") {
                        order.addItem(item, quantity);
                    } else {
                        cout << "Invalid item selection.\n";
                    }

                    cout << "Add more items? (y/n): ";
                    cin >> moreItems;
                } while (moreItems == 'y' || moreItems == 'Y');

                order.printBill(customerName);
                break;
            }

            case 5:
                sales->generateReport("Daily_Report.txt");
                break;

            case 0:
                cout << "Exiting...\n";
                break;

            default:
                cout << "Invalid choice.\n";
        }
    } while (choice != 0);

    delete menu;
    delete reservationSystem;
    delete sales;

    return 0;
}
