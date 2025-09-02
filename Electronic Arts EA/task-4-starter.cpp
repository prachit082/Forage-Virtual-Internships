#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <limits>

class Item {
private:
    std::string name;
    int quantity;
    float price;

public:
    Item(std::string name, int quantity, float price) :
        name{std::move(name)},
        quantity{quantity},
        price{price} {}

    std::string get_name() const {
        return name;
    }

    int get_quantity() const {
        return quantity;
    }

    void set_quantity(int new_quantity) {
        quantity = new_quantity;
    }

    float get_price() const {
        return price;
    }

    bool is_match(const std::string &other) const {
        return name == other;
    }
};

class Inventory {
private:
    // --- REFACTOR ---
    // Replaced C-style array of raw pointers with a vector of unique_ptr.
    std::vector<std::unique_ptr<Item>> items;
    float total_money;

    // A static helper function to display item data.
    // Made it const-correct by taking a const reference.
    static void display_data(const Item &item) {
        std::cout << "\nItem name: " << item.get_name();
        std::cout << "\nQuantity: " << item.get_quantity();
        std::cout << "\nPrice: " << item.get_price();
    }

public:
    Inventory() : total_money{0} {}

    void add_item() {
        std::string name;
        int quantity;
        float price;

        std::cout << "\nEnter item name: ";
        std::cin >> name;
        std::cout << "Enter quantity: ";
        std::cin >> quantity;
        std::cout << "Enter price: ";
        std::cin >> price;

        // --- REFACTOR ---
        // Using `emplace_back` with `std::make_unique` for modern, safe, and efficient addition.
        items.emplace_back(std::make_unique<Item>(name, quantity, price));
        std::cout << "\nItem added successfully.";
    }

    void sell_item() {
        if (items.empty()) {
            std::cout << "\nInventory is empty. Nothing to sell.";
            return;
        }

        std::string item_to_check;
        std::cout << "\nEnter item name to sell: ";
        std::cin >> item_to_check;

        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i]->is_match(item_to_check)) {
                remove_item(i); // Pass the index to the remove function
                return;
            }
        }
        std::cout << "\nThis item is not in your Inventory.";
    }

    void remove_item(size_t item_index) {
        int input_quantity;
        Item *item = items[item_index].get();

        std::cout << "\nEnter number of items to sell: ";
        std::cin >> input_quantity;

        int current_quantity = item->get_quantity();
        if (input_quantity > 0 && input_quantity <= current_quantity) {
            float price = item->get_price();
            float money_earned = price * input_quantity;

            int new_quantity = current_quantity - input_quantity;
            item->set_quantity(new_quantity);

            std::cout << "\nItems sold.";
            std::cout << "\nMoney received: " << money_earned;
            total_money += money_earned;

            // --- BUG FIX ---
            // If the item's quantity is now zero, completely remove it from the inventory.
            if (new_quantity == 0) {
                // `erase` removes the element from the vector. The unique_ptr's destructor
                // is automatically called, deleting the Item object and preventing memory leaks.
                items.erase(items.begin() + item_index);
                std::cout << "\nItem removed from inventory as quantity is now 0.";
            }

        } else if (input_quantity > current_quantity) {
            std::cout << "\nCannot sell more items than you have.";
        } else {
            std::cout << "\nInvalid quantity entered.";
        }
    }

    void list_items() const {
        if (items.empty()) {
            std::cout << "\nInventory is empty.";
            return;
        }

        std::cout << "\n--- Current Inventory ---";
        for (const auto &item : items) {
            display_data(*item);
            std::cout << "\n-------------------------";
        }
    }
};

int main() {
    int choice;
    Inventory inventory_system;
    std::cout << "Welcome to the inventory!";

    while (true) {
        std::cout << "\n\nMENU\n"
                  << "1. Add new item\n"
                  << "2. Sell item\n"
                  << "3. List items\n"
                  << "4. Exit\n\n"
                  << "Enter your choice: ";
        std::cin >> choice;

        // Checking for invalid input
        if (std::cin.fail()) {
            choice = 0; // Set choice to a value that will trigger the default case
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }


        switch (choice) {
            case 1:
                inventory_system.add_item();
                break;

            case 2:
                inventory_system.sell_item();
                break;

            case 3:
                inventory_system.list_items();
                break;

            case 4:
                exit(0);

            default:
                std::cout << "\nInvalid choice entered. Please try again.";
                break;
        }
    }
    // The return 0; is unreachable because of the infinite loop and exit(0)
    // but it's good practice for main to have a return statement.
    return 0;
}