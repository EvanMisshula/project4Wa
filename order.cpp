#include <iostream>
#include <cstring> // For strncpy

// Enum for order types
enum ordertype {LIMIT, MARKET};

// Base Order class
class Order {
protected:
    long timestamp;            // Epoch time
    bool is_buy;               // Buy or sell flag
    unsigned int id;           // Unique order ID
    unsigned int price;        // Price of the order
    unsigned int quantity;     // Quantity of the order
    char venue[20];            // Venue name
    char symbol[20];           // Symbol of the instrument
    ordertype type;            // Type of the order (LIMIT or MARKET)

public:
    // Constructor
    Order(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
          unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_)
        : timestamp(timestamp_), is_buy(is_buy_), id(id_), price(price_), quantity(quantity_), type(type_) {
        strncpy(venue, venue_, sizeof(venue) - 1);
        venue[sizeof(venue) - 1] = '\0';
        strncpy(symbol, symbol_, sizeof(symbol) - 1);
        symbol[sizeof(symbol) - 1] = '\0';
    }

    // Pure virtual function to get the outstanding quantity
    virtual unsigned int getOutstandingQuantity() const = 0;

    // Getters for other fields
    unsigned int getID() const { return id; }
    unsigned int getQuantity() const { return quantity; }
    unsigned int getPrice() const { return price; }
};

// Derived OpenOrder class
class OpenOrder : public Order {
public:
    OpenOrder(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
              unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

    unsigned int getOutstandingQuantity() const override {
        return quantity;
    }
};

// Derived ClosedOrder class
class ClosedOrder : public Order {
public:
    ClosedOrder(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
                unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

    unsigned int getOutstandingQuantity() const override {
        return 0;
    }
};

// VectorOrder class to manage an array of Order pointers
class VectorOrder {
private:
    Order** orders;               // Array of pointers to Order objects
    unsigned int capacity;        // Maximum number of orders the array can hold
    unsigned int current_new_order_offset;  // Tracks where the next new order should be placed

    // Helper function to resize the array when capacity is exceeded
    void resize() {
        capacity *= 2;
        Order** new_orders = new Order*[capacity];
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            new_orders[i] = orders[i];
        }
        delete[] orders;
        orders = new_orders;
    }

public:
    // Constructor
    VectorOrder(unsigned int initial_capacity = 10)
        : capacity(initial_capacity), current_new_order_offset(0) {
        orders = new Order*[capacity];
    }

    // Destructor to clean up allocated memory
    ~VectorOrder() {
        clear();
        delete[] orders;
    }

    // Add a new order to the array
    bool add_order(Order* o) {
        // Check if order ID already exists
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            if (orders[i]->getID() == o->getID()) {
                return false; // Order ID already exists
            }
        }

        // Resize the array if capacity is exceeded
        if (current_new_order_offset == capacity) {
            resize();
        }

        // Add the new order
        orders[current_new_order_offset++] = o;
        return true;
    }

    // Delete an order by its ID
    bool delete_order(unsigned int id) {
        bool found = false;
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            if (orders[i]->getID() == id) {
                found = true;
                delete orders[i]; // Free the memory
                // Shift the remaining orders to fill the gap
                for (unsigned int j = i; j < current_new_order_offset - 1; ++j) {
                    orders[j] = orders[j + 1];
                }
                --current_new_order_offset;
                break;
            }
        }
        return found;
    }

    // Clear all orders from the array
    void clear() {
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            delete orders[i];
        }
        current_new_order_offset = 0;
    }

    // Get total volume (sum of all order quantities)
    int get_total_volume() const {
        int total_volume = 0;
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            total_volume += orders[i]->getQuantity();
        }
        return total_volume;
    }

    // Get total outstanding volume
    int get_total_outstanding_volume() const {
        int total_outstanding_volume = 0;
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            total_outstanding_volume += orders[i]->getOutstandingQuantity();
        }
        return total_outstanding_volume;
    }

    // Print orders with outstanding volume
    void print_outstanding_orders() const {
        for (unsigned int i = 0; i < current_new_order_offset; ++i) {
            std::cout << "Order ID: " << orders[i]->getID()
                      << ", Outstanding Quantity: " << orders[i]->getOutstandingQuantity()
                      << std::endl;
        }
    }
};

// Main function to demonstrate functionality
int main() {
    VectorOrder manager(5);

    // Add some open and closed orders
    manager.add_order(new OpenOrder(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT));
    manager.add_order(new ClosedOrder(1633072900, false, 102, 150, 15, "NASDAQ", "GOOG", MARKET));

    // Print total volume
    std::cout << "Total Volume: " << manager.get_total_volume() << std::endl;

    // Print outstanding volume
    std::cout << "Outstanding Volume: " << manager.get_total_outstanding_volume() << std::endl;

    // Print outstanding orders
    manager.print_outstanding_orders();

    // Delete an order
    if (manager.delete_order(101)) {
        std::cout << "Order 101 deleted." << std::endl;
    }

    // Print total volume after deletion
    std::cout << "Total Volume after deletion: " << manager.get_total_volume() << std::endl;

    return 0;
}
