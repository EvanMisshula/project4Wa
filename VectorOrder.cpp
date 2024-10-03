#include "VectorOrder.h"
#include <iostream>

// Constructor
VectorOrder::VectorOrder(unsigned int initial_capacity)
    : capacity(initial_capacity), current_new_order_offset(0) {
    orders = new Order*[capacity];
}

// Destructor
VectorOrder::~VectorOrder() {
    clear();
    delete[] orders;
}

// Helper function to resize the array when capacity is exceeded
void VectorOrder::resize() {
    capacity *= 2;
    Order** new_orders = new Order*[capacity];
    for (unsigned int i = 0; i < current_new_order_offset; ++i) {
        new_orders[i] = orders[i];
    }
    delete[] orders;
    orders = new_orders;
}

// Add a new order
bool VectorOrder::add_order(Order* o) {
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

// Delete an order by ID
bool VectorOrder::delete_order(unsigned int id) {
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

// Clear all orders
void VectorOrder::clear() {
    for (unsigned int i = 0; i < current_new_order_offset; ++i) {
        delete orders[i];
    }
    current_new_order_offset = 0;
}

// Get total volume
int VectorOrder::get_total_volume() const {
    int total_volume = 0;
    for (unsigned int i = 0; i < current_new_order_offset; ++i) {
        total_volume += orders[i]->getQuantity();
    }
    return total_volume;
}

// Get total outstanding volume
int VectorOrder::get_total_outstanding_volume() const {
    int total_outstanding_volume = 0;
    for (unsigned int i = 0; i < current_new_order_offset; ++i) {
        total_outstanding_volume += orders[i]->getOutstandingQuantity();
    }
    return total_outstanding_volume;
}

// Print orders with outstanding volume
void VectorOrder::print_outstanding_orders() const {
    for (unsigned int i = 0; i < current_new_order_offset; ++i) {
        std::cout << "Order ID: " << orders[i]->getID()
                  << ", Outstanding Quantity: " << orders[i]->getOutstandingQuantity()
                  << std::endl;
    }
}
