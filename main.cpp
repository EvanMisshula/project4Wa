#include "VectorOrder.h"
#include "OpenOrder.h"
#include "ClosedOrder.h"
#include <iostream>

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
