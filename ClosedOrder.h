#ifndef CLOSEDORDER_H
#define CLOSEDORDER_H

#include "Order.h"

class ClosedOrder : public Order {
public:
    ClosedOrder(long timestamp_,
                bool is_buy_,
                unsigned int id_,
                unsigned int price_,
                unsigned int quantity_,
                const char* venue_,
                const char* symbol_,
                ordertype type_);

    unsigned int getOutstandingQuantity() const override;
};

#endif // CLOSEDORDER_H
