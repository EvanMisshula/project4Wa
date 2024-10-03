#ifndef ORDER_H
#define ORDER_H

#include <cstring> // For strncpy

// Enum for order types
enum ordertype { LIMIT, MARKET };

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
    Order(long timestamp_,
          bool is_buy_,
          unsigned int id_,
          unsigned int price_,
          unsigned int quantity_,
          const char* venue_,
          const char* symbol_,
          ordertype type_);

    // Pure virtual function to get the outstanding quantity
    virtual unsigned int getOutstandingQuantity() const = 0;

    // Getters
    unsigned int getID() const;
    unsigned int getQuantity() const;
    unsigned int getPrice() const;
};

#endif // ORDER_H
