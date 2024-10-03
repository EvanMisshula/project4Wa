#include "OpenOrder.h"

// Constructor for OpenOrder
OpenOrder::OpenOrder(long timestamp_,
                     bool is_buy_,
                     unsigned int id_,
                     unsigned int price_,
                     unsigned int quantity_,
                     const char* venue_,
                     const char* symbol_,
                     ordertype type_)
    : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

// Implementation of getOutstandingQuantity
unsigned int OpenOrder::getOutstandingQuantity() const {
    return quantity;
}
