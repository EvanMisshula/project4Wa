#ifndef OPENORDER_H
#define OPENORDER_H

#include "Order.h"

class OpenOrder : public Order {
public:
    OpenOrder(long timestamp_,
              bool is_buy_,
              unsigned int id_,
              unsigned int price_,
              unsigned int quantity_,
              const char* venue_,
              const char* symbol_,
              ordertype type_);

    unsigned int getOutstandingQuantity() const override;
};

#endif // OPENORDER_H
