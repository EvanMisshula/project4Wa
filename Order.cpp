#include "Order.h"

// Constructor
Order::Order(long timestamp_,
             bool is_buy_,
             unsigned int id_,
             unsigned int price_,
             unsigned int quantity_,
             const char* venue_,
             const char* symbol_,
             ordertype type_)
    : timestamp(timestamp_), is_buy(is_buy_), id(id_), price(price_), quantity(quantity_), type(type_) {
    strncpy(this->venue, venue_, sizeof(this->venue) - 1);
    this->venue[sizeof(this->venue) - 1] = '\0';
    strncpy(this->symbol, symbol_, sizeof(this->symbol) - 1);
    this->symbol[sizeof(this->symbol) - 1] = '\0';
}

// Getters
unsigned int Order::getID() const { return id; }
unsigned int Order::getQuantity() const { return quantity; }
unsigned int Order::getPrice() const { return price; }
