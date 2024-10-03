#ifndef VECTORORDER_H
#define VECTORORDER_H

#include "Order.h" // Ensure Order.h is included because VectorOrder works with Order objects

class VectorOrder {
private:
    Order** orders;                // Array of pointers to Order objects
    unsigned int capacity;         // Maximum number of orders
    unsigned int current_new_order_offset;  // Current offset for the next new order

    // Helper function to resize the array when capacity is exceeded
    void resize();

public:
    // Constructor
    VectorOrder(unsigned int initial_capacity = 10);

    // Destructor
    ~VectorOrder();

    // Add a new order
    bool add_order(Order* o);

    // Delete an order by ID
    bool delete_order(unsigned int id);

    // Clear all orders
    void clear();

    // Get total volume
    int get_total_volume() const;

    // Get total outstanding volume
    int get_total_outstanding_volume() const;

    // Print orders with outstanding volume
    void print_outstanding_orders() const;
};

#endif // VECTORORDER_H
    
