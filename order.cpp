#include <iostream>
#include <vector>

// Enum representing order types
enum ordertype {LIMIT, MARKET};

// Base class Order
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

    // Virtual method to get outstanding quantity
    virtual unsigned int getOutstandingQuantity() const = 0;

    // Getter methods
    unsigned int getID() const { return id; }
    unsigned int getQuantity() const { return quantity; }
    unsigned int getPrice() const { return price; }
};

// Derived class OpenOrder
class OpenOrder : public Order {
public:
    OpenOrder(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
              unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

    // Open orders have outstanding quantity equal to their current quantity
    unsigned int getOutstandingQuantity() const override {
        return quantity;
    }
};

// Derived class ClosedOrder
class ClosedOrder : public Order {
public:
    ClosedOrder(long timestamp_, bool is_buy_, unsigned int id_, unsigned int price_,
                unsigned int quantity_, const char* venue_, const char* symbol_, ordertype type_)
        : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

    // Closed orders have no outstanding quantity
    unsigned int getOutstandingQuantity() const override {
        return 0;
    }
};

// Container class VectorOrder to manage orders
class VectorOrder {
private:
    std::vector<Order*> orders;

public:
    ~VectorOrder() {
        for (Order* order : orders) {
            delete order;
        }
    }

    void addOrder(Order* order) {
        orders.push_back(order);
    }

    // Print all orders with outstanding quantities
    void printOutstandingOrders() const {
        for (const Order* order : orders) {
            std::cout << "Order ID: " << order->getID() << ", Outstanding Quantity: " << order->getOutstandingQuantity() << std::endl;
        }
    }
};

int main() {
    VectorOrder manager;

    // Adding an open order
    manager.addOrder(new OpenOrder(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT));
    
    // Adding a closed order
    manager.addOrder(new ClosedOrder(1633072900, false, 102, 150, 15, "NASDAQ", "GOOG", MARKET));

    // Print outstanding orders
    manager.printOutstandingOrders();

    return 0;
}
