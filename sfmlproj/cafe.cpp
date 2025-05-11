#include "cafe.h"
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>

// Button implementation
Button::Button(const std::string& text, float x, float y, sf::Font& font) {
    buttonText.setFont(font);
    buttonText.setString(text);
    buttonText.setCharacterSize(24);
    buttonText.setFillColor(sf::Color(203, 178, 167));

    float paddingX = 20.f;
    float paddingY = 10.f;

    sf::FloatRect textBounds = buttonText.getLocalBounds();
    buttonShape.setSize(sf::Vector2f(textBounds.width + 2 * paddingX, textBounds.height + 2 * paddingY));
    buttonShape.setPosition(x, y);
    buttonShape.setFillColor(sf::Color(62, 39, 35));

    buttonText.setPosition(
        x + paddingX - textBounds.left,
        y + paddingY - textBounds.top
    );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(buttonShape);
    window.draw(buttonText);
}

bool Button::isClicked(const sf::Vector2i& mousePosition) {
    return buttonShape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosition));
}

// Cafe implementation
Cafe::Cafe() : currentScreen(Screen::Home), selectedItemIndex(-1) {
    font.loadFromFile("lorabold.ttf");

    viewMenuButton = Button("1. View Menu", 200, 150, font);
    placeOrderButton = Button("2. Place Order", 200, 200, font);
    reserveTableButton = Button("3. Reserve Table", 200, 250, font);
    viewReservationsButton = Button("4. View Reservations", 200, 300, font);
    backButton = Button("Back", 200, 500, font);
    checkoutButton = Button("Checkout", 350, 500, font);

    loadMenu();
    reservationErrorMessage = "";
}

void Cafe::loadMenu() {
    std::ifstream file("menu.txt");
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        MenuItem item;
        if (iss >> item.name >> item.price) {
            menuItems.push_back(item);
        }
    }
}

void Cafe::handleEvent(const sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape) {
            currentScreen = Screen::Home;
            quantityInput.clear();
            selectedItemIndex = -1;
            currentInput.clear();
            tableInput.clear();
            nameEntered = false;
            reservationErrorMessage.clear();
        }
    }

    if (currentScreen == Screen::Order && selectedItemIndex >= 0) {
        if (event.type == sf::Event::TextEntered) {
            if (std::isdigit(event.text.unicode)) {
                quantityInput += static_cast<char>(event.text.unicode);
            } else if (event.text.unicode == '\b' && !quantityInput.empty()) {
                quantityInput.pop_back();
            } else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
                if (!quantityInput.empty()) {
                    OrderItem order;
                    order.item = menuItems[selectedItemIndex];
                    order.quantity = std::stoi(quantityInput);
                    orderItems.push_back(order);
                }
                quantityInput.clear();
                selectedItemIndex = -1;
            }
        }
    }

    if (currentScreen == Screen::Reservation) {
        if (event.type == sf::Event::TextEntered) {
            char unicodeChar = static_cast<char>(event.text.unicode);
            if (!nameEntered) {
                if (std::isprint(unicodeChar)) currentInput += unicodeChar;
                if (event.text.unicode == '\b' && !currentInput.empty()) currentInput.pop_back();
                if (event.text.unicode == '\r' || event.text.unicode == '\n') nameEntered = true;
            } else {
                if (std::isdigit(unicodeChar)) tableInput += unicodeChar;
                if (event.text.unicode == '\b' && !tableInput.empty()) tableInput.pop_back();
                if ((event.text.unicode == '\r' || event.text.unicode == '\n') && !tableInput.empty()) {
                    int tableNum = std::stoi(tableInput);
                    if (tableNum >= 1 && tableNum <= 10) {
                        if (reservations.count(tableNum) == 0) {
                            reservations[tableNum] = currentInput;
                            reservationErrorMessage.clear();
                            currentInput.clear();
                            tableInput.clear();
                            nameEntered = false;
                            currentScreen = Screen::Home;
                        } else {
                            reservationErrorMessage = "Table " + std::to_string(tableNum) + " is already reserved!";
                            tableInput.clear();
                        }
                    } else {
                        reservationErrorMessage = "Invalid table number. Choose 1 to 10.";
                        tableInput.clear();
                    }
                }
            }
        }
    }

    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);

        if (viewMenuButton.isClicked(mousePos)) currentScreen = Screen::Menu;
        else if (placeOrderButton.isClicked(mousePos)) currentScreen = Screen::Order;
        else if (reserveTableButton.isClicked(mousePos)) currentScreen = Screen::Reservation;
        else if (viewReservationsButton.isClicked(mousePos)) currentScreen = Screen::ViewReservations;
        else if (backButton.isClicked(mousePos)) {
            if (currentScreen == Screen::Bill) orderItems.clear();
            currentScreen = Screen::Home;
            reservationErrorMessage.clear();
        }
        else if (checkoutButton.isClicked(mousePos) && !orderItems.empty()) {
            processOrder();
            currentScreen = Screen::Bill;
        }

        if (currentScreen == Screen::Order) {
            float yOffset = 100;
            for (size_t i = 0; i < menuItems.size(); ++i) {
                sf::FloatRect rect(200, yOffset, 400, 30);
                if (rect.contains(static_cast<sf::Vector2f>(mousePos))) {
                    selectedItemIndex = static_cast<int>(i);
                    quantityInput.clear();
                }
                yOffset += 30;
            }
        }
    }
}

void Cafe::draw(sf::RenderWindow& window) {
    switch (currentScreen) {
        case Screen::Home: drawHome(window); break;
        case Screen::Menu: drawMenu(window); break;
        case Screen::Order: drawOrder(window); break;
        case Screen::Reservation: drawReservation(window); break;
        case Screen::ViewReservations: drawViewReservations(window); break;
        case Screen::Bill: drawBill(window); break;
    }
}

void Cafe::drawHome(sf::RenderWindow& window) {
    sf::Text title("Cafe Management System", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    viewMenuButton.draw(window);
    placeOrderButton.draw(window);
    reserveTableButton.draw(window);
    viewReservationsButton.draw(window);
}

void Cafe::drawMenu(sf::RenderWindow& window) {
    sf::Text title("Menu", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    float yOffset = 100;
    for (const auto& item : menuItems) {
        std::ostringstream ss;
        ss << item.name << " - $" << std::fixed << std::setprecision(2) << item.price;
        sf::Text text(ss.str(), font, 20);
        text.setPosition(200, yOffset);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
        yOffset += 30;
    }

    backButton.draw(window);
}

void Cafe::drawOrder(sf::RenderWindow& window) {
    sf::Text title("Place Order", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    float yOffset = 100;
    for (size_t i = 0; i < menuItems.size(); ++i) {
        std::ostringstream ss;
        ss << i + 1 << ". " << menuItems[i].name << " - $" << std::fixed << std::setprecision(2) << menuItems[i].price;
        sf::Text text(ss.str(), font, 20);
        text.setPosition(200, yOffset);
        text.setFillColor(i == selectedItemIndex ? sf::Color::Red : sf::Color::Black);
        window.draw(text);
        yOffset += 30;
    }

    if (selectedItemIndex >= 0) {
        sf::Text prompt("Enter quantity: " + quantityInput, font, 20);
        prompt.setPosition(200, yOffset + 20);
        prompt.setFillColor(sf::Color::Blue);
        window.draw(prompt);
    }

    if (!orderItems.empty())
        checkoutButton.draw(window);

    backButton.draw(window);
}

void Cafe::drawReservation(sf::RenderWindow& window) {
    sf::Text title("Reserve Table", font, 30);
    title.setPosition(200, 30);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    float y = 100;
    for (int i = 1; i <= 10; ++i) {
        std::ostringstream ss;
        ss << "Table " << i << ": ";
        if (reservations.count(i)) {
            ss << "Reserved by " << reservations[i];
        } else {
            ss << "Available";
        }

        sf::Text line(ss.str(), font, 20);
        line.setPosition(200, y);
        line.setFillColor(reservations.count(i) ? sf::Color::Red : sf::Color::Green);
        window.draw(line);
        y += 30;
    }

    if (!nameEntered) {
        sf::Text prompt("Enter your name: " + currentInput, font, 20);
        prompt.setPosition(200, y + 20);
        prompt.setFillColor(sf::Color::Black);
        window.draw(prompt);
    } else {
        sf::Text prompt("Enter table number (1-10): " + tableInput, font, 20);
        prompt.setPosition(200, y + 20);
        prompt.setFillColor(sf::Color::Black);
        window.draw(prompt);
    }

    if (!reservationErrorMessage.empty()) {
        sf::Text errorText(reservationErrorMessage, font, 18);
        errorText.setPosition(200, y + 60);
        errorText.setFillColor(sf::Color::Red);
        window.draw(errorText);
    }

    backButton.draw(window);
}

void Cafe::drawViewReservations(sf::RenderWindow& window) {
    sf::Text title("Current Reservations", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    float y = 100;
    for (const auto& res : reservations) {
        std::ostringstream ss;
        ss << "Table " << res.first << ": Reserved by " << res.second;
        sf::Text line(ss.str(), font, 20);
        line.setPosition(200, y);
        line.setFillColor(sf::Color::Black);
        window.draw(line);
        y += 30;
    }

    backButton.draw(window);
}

void Cafe::drawBill(sf::RenderWindow& window) {
    sf::Text title("Order Receipt", font, 30);
    title.setPosition(200, 50);
    title.setFillColor(sf::Color::Black);
    window.draw(title);

    float yOffset = 100;
    float total = 0;
    for (const auto& item : orderItems) {
        float subtotal = item.quantity * item.item.price;
        total += subtotal;
        std::ostringstream ss;
        ss << item.item.name << " x" << item.quantity << " = $" << std::fixed << std::setprecision(2) << subtotal;
        sf::Text text(ss.str(), font, 20);
        text.setPosition(200, yOffset);
        text.setFillColor(sf::Color::Black);
        window.draw(text);
        yOffset += 30;
    }

    std::ostringstream totalSS;
    totalSS << "Total: $" << std::fixed << std::setprecision(2) << total;
    sf::Text totalText(totalSS.str(), font, 24);
    totalText.setPosition(200, yOffset + 20);
    totalText.setFillColor(sf::Color::Black);
    window.draw(totalText);

    backButton.draw(window);
}

void Cafe::processOrder() {
    std::ofstream bill("bill.txt");
    float total = 0;
    bill << "----- Cafe Receipt -----\n";
    for (const auto& item : orderItems) {
        float subtotal = item.quantity * item.item.price;
        total += subtotal;
        bill << item.item.name << " x" << item.quantity << " = $" << std::fixed << std::setprecision(2) << subtotal << "\n";
    }
    bill << "------------------------\n";
    bill << "Total: $" << std::fixed << std::setprecision(2) << total << "\n";
    bill.close();
}
