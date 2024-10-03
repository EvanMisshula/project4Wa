#include "ClosedOrder.h"

// Constructor for ClosedOrder
ClosedOrder::ClosedOrder(long timestamp_,
                         bool is_buy_,
                         unsigned int id_,
                         unsigned int price_,
                         unsigned int quantity_,
                         const char* venue_,
                         const char* symbol_,
                         ordertype type_)
    : Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_) {}

// Implementation of getOutstandingQuantity for ClosedOrder
unsigned int ClosedOrder::getOutstandingQuantity() const {
    return 0; // Closed orders have no outstanding quantity
}
