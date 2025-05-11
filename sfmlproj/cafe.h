#ifndef CAFE_H
#define CAFE_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>

struct MenuItem {
    std::string name;
    float price;
};

struct OrderItem {
    MenuItem item;
    int quantity;
};

class Button {
public:
    Button() = default;
    Button(const std::string& text, float x, float y, sf::Font& font);
    void draw(sf::RenderWindow& window);
    bool isClicked(const sf::Vector2i& mousePosition);

private:
    sf::RectangleShape buttonShape;
    sf::Text buttonText;
};

class Cafe {
public:
    Cafe();

    void handleEvent(const sf::Event& event, sf::RenderWindow& window);
    void draw(sf::RenderWindow& window);

private:
    enum class Screen {
        Home, Menu, Order, Reservation, ViewReservations, Bill
    };

    sf::Font font;
    Screen currentScreen;

    std::vector<MenuItem> menuItems;
    std::vector<OrderItem> orderItems;
    std::map<int, std::string> reservations;

    Button viewMenuButton;
    Button placeOrderButton;
    Button reserveTableButton;
    Button viewReservationsButton;
    Button backButton;
    Button checkoutButton;

    int selectedItemIndex;
    std::string quantityInput;

    std::string currentInput;
    std::string tableInput; 
    std::string reservationErrorMessage; 
    int selectedTable = -1;
    bool nameEntered = false;

    void loadMenu();
    void processOrder();

    void drawHome(sf::RenderWindow& window);
    void drawMenu(sf::RenderWindow& window);
    void drawOrder(sf::RenderWindow& window);
    void drawReservation(sf::RenderWindow& window);
    void drawViewReservations(sf::RenderWindow& window);
    void drawBill(sf::RenderWindow& window);
};

#endif
