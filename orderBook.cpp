#include <iostream>
#include <cstring>  // For strcpy and strncpy

// Enum representing order types
enum ordertype {LIMIT, MARKET};

// Order class definition
class Order {
private:
    long timestamp;             // Epoch time
    bool is_buy;                // Buy or sell flag
    unsigned int id;            // Unique order ID
    unsigned int price;         // Price of the order
    unsigned int quantity;      // Quantity of the order
    char venue[20];             // Venue name
    char symbol[20];            // Symbol of the instrument
    ordertype type;             // Type of the order (LIMIT or MARKET)

public:
    // Constructor
    Order(long timestamp_,
	  bool is_buy_,
	  unsigned int id_,
	  unsigned int price_,
          unsigned int quantity_,
	  const char* venue_,
	  const char* symbol_,
	  ordertype type_)
        : timestamp(timestamp_), is_buy(is_buy_), id(id_), price(price_), 
          quantity(quantity_), type(type_) 
    {
        strncpy(venue, venue_, sizeof(venue) - 1);
        venue[sizeof(venue) - 1] = '\0';  // Ensure null termination

        strncpy(symbol, symbol_, sizeof(symbol) - 1);
        symbol[sizeof(symbol) - 1] = '\0';  // Ensure null termination
    }

    // Destructor
    ~Order() {
        std::cout << "Order with ID " << id << " destroyed." << std::endl;
    }

    // Getter for venue
    const char* getVenue() {
        return venue;
    }

    // Getter for symbol
    const char* getSymbol() {
        return symbol;
    }

    // Getter for ID
    unsigned int getID() {
        return id;
    }

    // Getter for order type
    ordertype getOrderType() {
        return type;
    }

    // Getter for quantity
    unsigned int getQuantity() {
        return quantity;
    }

    // Getter for price
    unsigned int getPrice() {
        return price;
    }

    // Validation function
      bool is_valid() {
        // Validate price and quantity for LIMIT orders (price should be > 0 for LIMIT orders)
        if (type == LIMIT && price <= 0) {
            return false;
        }

        // Quantity must always be greater than 0
        if (quantity <= 0) {
            return false;
        }

        // Venue must be a non-empty string
        if (venue[0] == '\0') {
            return false;
        }

        // Symbol must be a non-empty string
        if (symbol[0] == '\0') {
            return false;
        }

        // If all checks pass, the order is valid
        return true;
    }

    // Setter for venue
    void setVenue(const char* venue_) {
        strncpy(venue, venue_, sizeof(venue) - 1);
        venue[sizeof(venue) - 1] = '\0';  // Ensure null termination
    }


// Function to safely assign an unsigned integer
void setUnsignedValue(unsigned int &uvalue, int input) {
    // Check if the input is negative
    if (input < 0) {
        std::cout << "Negative input given. Nothing will be assigned.\n";
        return;  // Early exit if the input is negative
    }
    
    // Safely assign input to the unsigned value if it is non-negative
    uvalue = static_cast<unsigned int>(input);
    std::cout << "Value assigned: " << uvalue << std::endl;
}
    // Setter for quantity
    void setQuantity(int input_quantity) {
	if (input_quantity < 0 ){
	    std::cout << "Negative input given. Nothing will be assigned.\n";
	    return;  // Early exit if the input is negative
	} else {
	    quantity = static_cast<unsigned int>(input_quantity);    
	}
	
	return;
    }
};

// OrderBook class definition
class OrderBook {
private:
    // Arrays to hold pointers to orders
    Order* order_bids[20];       // Bid orders
    Order* order_offers[20];     // Offer orders

    int number_of_bids;          // Number of bids
    int number_of_offers;        // Number of offers

public:
    // Constructor
    OrderBook() : number_of_bids(0), number_of_offers(0) {
        // Initialize the arrays with nullptr
        for (int i = 0; i < 20; ++i) {
            order_bids[i] = nullptr;
            order_offers[i] = nullptr;
        }
    }

    // Destructor
    ~OrderBook() {
        // Delete all dynamically allocated bid orders
        for (int i = 0; i < number_of_bids; ++i) {
            delete order_bids[i];
        }
        
        // Delete all dynamically allocated offer orders
        for (int i = 0; i < number_of_offers; ++i) {
            delete order_offers[i];
        }

    }

    // Add an order
    bool add_order(long timestamp_,
		   bool is_buy_,
		   unsigned int id_,
		   unsigned int price_,
                   unsigned int quantity_,
		   const char* venue_,
		   const char* symbol_,
		   ordertype type_) {
        if (is_buy_ && number_of_bids < 20) {
	    Order * temp = new Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_);
	    if(temp->is_valid()) {
		order_bids[number_of_bids++] = temp;
		return true;
	    }
        } else if (!is_buy_ && number_of_offers < 20) {
	    Order * temp = new Order(timestamp_, is_buy_, id_, price_, quantity_, venue_, symbol_, type_);
            if(temp->is_valid()) {
		order_offers[number_of_offers++] = temp;
		return true;
	    }
	    
        }
        return false;
    }

    // Modify an order's quantity
    bool modify_order(bool is_buy_, unsigned int id_, unsigned int quantity_) {
        Order** orders = is_buy_ ? order_bids : order_offers;
        int size = is_buy_ ? number_of_bids : number_of_offers;

        for (int i = 0; i < size; ++i) {
            if (orders[i]->getID() == id_) {
                orders[i]->setQuantity(quantity_);
                return true;
            }
        }
        return false;
    }

    // Remove an order
    bool remove_order(bool is_buy_, unsigned int id_) {
        Order** orders = is_buy_ ? order_bids : order_offers;
        int& size = is_buy_ ? number_of_bids : number_of_offers;

        for (int i = 0; i < size; ++i) {
            if (orders[i]->getID() == id_) {
                delete orders[i];
                orders[i] = orders[--size];  // Move last order to this position
                orders[size] = nullptr;
                return true;
            }
        }
        return false;
    }

    // Utility functions
    void clearBooks() {
        for (int i = 0; i < number_of_bids; ++i) {
            delete order_bids[i];
            order_bids[i] = nullptr;
        }
        number_of_bids = 0;

        for (int i = 0; i < number_of_offers; ++i) {
            delete order_offers[i];
            order_offers[i] = nullptr;
        }
        number_of_offers = 0;
    }

    // Getters for orders
    Order* getOrderBids(const int a) {
        return (a < number_of_bids && a >= 0) ? order_bids[a] : nullptr;
    }

    Order* getOrderOffers(const int a) {
        return (a < number_of_offers && a >= 0) ? order_offers[a] : nullptr;
    }

    int getNumberOfBids() {
        return number_of_bids;
    }

    int getNumberOfOffers() {
        return number_of_offers;
    }
};

void test_create_order() {
    Order order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    std::cout << "Test Create Order:" << std::endl;
    std::cout << "Order ID: " << order.getID() << " (Expected: 101)" << std::endl;
    std::cout << "Price: " << order.getPrice() << " (Expected: 200)" << std::endl;
    std::cout << "Quantity: " << order.getQuantity() << " (Expected: 10)" << std::endl;
    std::cout << "Venue: " << order.getVenue() << " (Expected: NYSE)" << std::endl;
}

void test_add_order() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    std::cout << "Test Add Order:" << std::endl;
    std::cout << "Number of Bids: " << ob.getNumberOfBids() << " (Expected: 1)" << std::endl;
}
void test_add_same_order_twice() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    bool second_add = ob.add_order(1633072900, true, 101, 200, 10, "NASDAQ", "GOOG", LIMIT);
    std::cout << "Test Add Same Order Twice:" << std::endl;
    std::cout << "Second Add Result: " << (second_add ? "Success" : "Failure (Expected)") << std::endl;
    std::cout << "Number of Bids: " << ob.getNumberOfBids() << " (Expected: 1)" << std::endl;
}
void test_add_negative_price_order() {
    OrderBook ob;
    bool add_result = ob.add_order(1633072800, true, 101, -200, 10, "NYSE", "AAPL", LIMIT);
    std::cout << "Test Add Negative Price Order:" << std::endl;
    std::cout << "Add Result: " << (add_result ? "Success" : "Failure (Expected)") << std::endl;
}
void test_remove_order() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.remove_order(true, 101);
    std::cout << "Test Remove Order:" << std::endl;
    std::cout << "Number of Bids After Removal: " << ob.getNumberOfBids() << " (Expected: 0)" << std::endl;
}
void test_modify_order_quantity() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.modify_order(true, 101, 25);
    Order* modified_order = ob.getOrderBids(0);
    std::cout << "Test Modify Order Quantity:" << std::endl;
    std::cout << "Modified Order Quantity: " << modified_order->getQuantity() << " (Expected: 25)" << std::endl;
}

void test_clear_books() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.add_order(1633072900, false, 102, 150, 15, "NASDAQ", "GOOG", MARKET);
    ob.clearBooks();
    std::cout << "Test Clear Books:" << std::endl;
    std::cout << "Number of Bids After Clear: " << ob.getNumberOfBids() << " (Expected: 0)" << std::endl;
    std::cout << "Number of Offers After Clear: " << ob.getNumberOfOffers() << " (Expected: 0)" << std::endl;
}
void test_orderbook_constructor() {
    OrderBook ob;
    std::cout << "Test OrderBook Constructor:" << std::endl;
    std::cout << "Initial Number of Bids: " << ob.getNumberOfBids() << " (Expected: 0)" << std::endl;
    std::cout << "Initial Number of Offers: " << ob.getNumberOfOffers() << " (Expected: 0)" << std::endl;
}
void test_is_valid() {
    Order valid_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    Order invalid_order(1633072800, true, 102, -200, 10, "NYSE", "AAPL", LIMIT);
    std::cout << "Test is_valid Function:" << std::endl;
    std::cout << "Valid Order is " << (valid_order.is_valid() ? "Valid" : "Invalid") << " (Expected: Valid)" << std::endl;
    std::cout << "Invalid Order is " << (invalid_order.is_valid() ? "Valid" : "Invalid") << " (Expected: Invalid)" << std::endl;
}
void test_add_order_with_existing_id() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    bool add_second = ob.add_order(1633072900, true, 101, 150, 15, "NASDAQ", "GOOG", MARKET);
    std::cout << "Test Add Order with Existing ID:" << std::endl;
    std::cout << "Second Add with Same ID Result: " << (add_second ? "Success" : "Failure (Expected)") << std::endl;
}
void test_modify_quantity_only() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    Order* original_order = ob.getOrderBids(0);
    ob.modify_order(true, 101, 25);

    std::cout << "Test Modify Quantity Only:" << std::endl;
    std::cout << "Original Price: " << original_order->getPrice() << " (Expected: 200)" << std::endl;
    std::cout << "Original Venue: " << original_order->getVenue() << " (Expected: NYSE)" << std::endl;
}
void test_get_number_of_bids() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.add_order(1633072900, true, 102, 250, 15, "NASDAQ", "MSFT", LIMIT);
    std::cout << "Test Get Number of Bids:" << std::endl;
    std::cout << "Number of Bids: " << ob.getNumberOfBids() << " (Expected: 2)" << std::endl;
}
void test_remove_nonexistent_order() {
    OrderBook ob;
    bool remove_result = ob.remove_order(true, 999);  // Non-existent order ID
    std::cout << "Test Remove Non-existent Order:" << std::endl;
    std::cout << "Remove Result: " << (remove_result ? "Success" : "Failure (Expected)") << std::endl;
}
void test_get_order_bids() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    Order* order = ob.getOrderBids(0);

    std::cout << "Test Get Order Bids:" << std::endl;
    if (order != nullptr) {
        std::cout << "Order ID: " << order->getID() << " (Expected: 101)" << std::endl;
    } else {
        std::cout << "No order found (Expected: Order ID 101)" << std::endl;
    }
}
void test_venue_set_correctly() {
    Order order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    order.setVenue("NASDAQ");
    
    std::cout << "Test Venue Set Correctly:" << std::endl;
    std::cout << "Venue: " << order.getVenue() << " (Expected: NASDAQ)" << std::endl;
}
void test_order_destructor() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    std::cout << "Test Order Destructor:" << std::endl;
    ob.remove_order(true, 101);  // Destructor will be called here
}
void test_get_order_offers_empty() {
    OrderBook ob;
    std::cout << "Test Get Order Offers When Empty:" << std::endl;
    Order* order = ob.getOrderOffers(0);
    std::cout << "Order: " << (order ? "Exists" : "Null (Expected)") << std::endl;
}
void test_add_order_with_empty_symbol() {
    OrderBook ob;
    bool add_result = ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "", LIMIT);  // Empty symbol
    std::cout << "Test Add Order with Empty Symbol:" << std::endl;
    std::cout << "Add Result: " << (add_result ? "Success" : "Failure (Expected)") << std::endl;
}
void test_modify_order_does_not_change_price() {
    OrderBook ob;
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.modify_order(true, 101, 25);  // Change quantity only
    Order* order = ob.getOrderBids(0);

    std::cout << "Test Modify Order Does Not Change Price:" << std::endl;
    std::cout << "Order Price: " << order->getPrice() << " (Expected: 200)" << std::endl;
}
void test_add_market_order() {
    OrderBook ob;
    bool add_result = ob.add_order(1633072900, true, 102, 0, 10, "NASDAQ", "GOOG", MARKET);  // Market order
    std::cout << "Test Add Market Order:" << std::endl;
    std::cout << "Add Result: " << (add_result ? "Success (Expected)" : "Failure") << std::endl;
}

// Main function to test the Order class and OrderBook class
int main() {
    // Create an order object
    Order order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);

    // Test getters
    std::cout << "Order ID: " << order.getID() << std::endl;
    std::cout << "Venue: " << order.getVenue() << std::endl;
    std::cout << "Symbol: " << order.getSymbol() << std::endl;
    std::cout << "Price: " << order.getPrice() << std::endl;
    std::cout << "Quantity: " << order.getQuantity() << std::endl;
    std::cout << "Order Type: " << (order.getOrderType() == LIMIT ? "LIMIT" : "MARKET") << std::endl;

    // Test if the order is valid
    if (order.is_valid()) {
        std::cout << "The order is valid." << std::endl;
    } else {
        std::cout << "The order is invalid." << std::endl;
    }

    // Test setters
    order.setVenue("NASDAQ");
    order.setQuantity(20);

    std::cout << "Updated Venue: " << order.getVenue() << std::endl;
    std::cout << "Updated Quantity: " << order.getQuantity() << std::endl;

    // Check if order is still valid after modifications
    if (order.is_valid()) {
        std::cout << "The updated order is valid." << std::endl;
    } else {
        std::cout << "The updated order is invalid." << std::endl;
    }

    // Create and test the OrderBook
    OrderBook ob;

    // Add orders
    ob.add_order(1633072800, true, 101, 200, 10, "NYSE", "AAPL", LIMIT);
    ob.add_order(1633072900, false, 102, 150, 15, "NASDAQ", "GOOG", MARKET);

    // Modify an order
    ob.modify_order(true, 101, 25);

    // Remove an order
    ob.remove_order(false, 102);

    // Clear the books
    ob.clearBooks();

    test_create_order();
    test_add_order();
    test_add_same_order_twice();
    test_add_negative_price_order();
    test_remove_order();
    test_modify_order_quantity();
    test_clear_books();
    test_orderbook_constructor();
    test_is_valid();
    test_add_order_with_existing_id();
      // Additional Tests
    test_modify_quantity_only();
    test_get_number_of_bids();
    test_remove_nonexistent_order();
    test_get_order_bids();
    test_venue_set_correctly();
    test_order_destructor();
    test_get_order_offers_empty();
    test_add_order_with_empty_symbol();
    test_modify_order_does_not_change_price();
    test_add_market_order();
    // order snippets
     std::cout << "\n--- TEST 1: Get Order Quantity ---" << std::endl;
    Order order1(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    std::cout << "Order Quantity: " << order1.getQuantity() << " (Expected: 50)" << std::endl;

    std::cout << "\n--- TEST 2: Validate Order ---" << std::endl;
    Order order2(1622505600, false, 2, 200, 25, "NASDAQ", "GOOG", MARKET);
    std::cout << "Is Order Valid: " << (order2.is_valid() ? "True" : "False") << " (Expected: True)" << std::endl;

    std::cout << "\n--- TEST 3: Validate Invalid Order (negative price) ---" << std::endl;
    Order order3(1622505600, true, 1, 100, -10, "NYSE", "AAPL", LIMIT);
    std::cout << "Is Order Valid: " << (order3.is_valid() ? "True" : "False") << " (Expected: False)" << std::endl;

    std::cout << "\n--- TEST 4: Modify Order Quantity ---" << std::endl;
    Order order4(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    order4.setQuantity(60);
    std::cout << "Modified Order Quantity: " << order4.getQuantity() << " (Expected: 60)" << std::endl;

    std::cout << "\n--- TEST 5: Validate Invalid Order (negative quantity) ---" << std::endl;
    Order order5(1622505600, true, 1, -100, 50, "NYSE", "AAPL", LIMIT);
    std::cout << "Is Order Valid: " << (order5.is_valid() ? "True" : "False") << " (Expected: False)" << std::endl;

    std::cout << "\n--- TEST 6: Get Order Symbol ---" << std::endl;
    Order order6(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    std::cout << "Order Symbol: " << order6.getSymbol() << " (Expected: AAPL)" << std::endl;

    std::cout << "\n--- TEST 7: Modify Order Venue ---" << std::endl;
    Order order7(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    order7.setVenue("LSE");
    std::cout << "Modified Order Venue: " << order7.getVenue() << " (Expected: LSE)" << std::endl;

    // Test 3: Empty OrderBook Bids
    std::cout << "\n--- TEST 3: Empty OrderBook Bids ---" << std::endl;
    OrderBook book;
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 0)" << std::endl;

    // Test 6: Add Order to OrderBook
    std::cout << "\n--- TEST 6: Add Order to OrderBook ---" << std::endl;
    book.add_order(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 1)" << std::endl;

    // Test 7: Remove Order from OrderBook
    std::cout << "\n--- TEST 7: Remove Order from OrderBook ---" << std::endl;
    book.remove_order(true, 1);
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 0)" << std::endl;

    // Test 8: Add Offer to OrderBook
    std::cout << "\n--- TEST 8: Add Offer to OrderBook ---" << std::endl;
    book.add_order(1622505600, false, 2, 200, 75, "NASDAQ", "GOOG", LIMIT);
    std::cout << "Number of Offers: " << book.getNumberOfOffers() << " (Expected: 1)" << std::endl;

    // Test 9: Modify Order in OrderBook
    std::cout << "\n--- TEST 9: Modify Order in OrderBook ---" << std::endl;
    book.add_order(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    book.modify_order(true, 1, 80);
    std::cout << "Modified Order Quantity: " << book.getOrderBids(0)->getQuantity() << " (Expected: 80)" << std::endl;

    // Test 10: Multiple Orders in OrderBook
    std::cout << "\n--- TEST 10: Multiple Orders in OrderBook ---" << std::endl;
    book.clearBooks();	
    book.add_order(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    book.add_order(1622505601, true, 1, 150, 30, "NYSE", "AAPL", LIMIT);
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 2)" << std::endl;

    // Test 12: Get Price of Bid
    std::cout << "\n--- TEST 12: Get Price of Bid ---" << std::endl;
    std::cout << "Price of Second Bid: " << book.getOrderBids(1)->getPrice() << " (Expected: 150)" << std::endl;

    // Test 13: Remove Offer from OrderBook
    std::cout << "\n--- TEST 13: Remove Offer from OrderBook ---" << std::endl;
    book.remove_order(false, 2);
    std::cout << "Number of Offers: " << book.getNumberOfOffers() << " (Expected: 0)" << std::endl;

    // Test 14: Clear OrderBook
    std::cout << "\n--- TEST 14: Clear OrderBook ---" << std::endl;
    book.clearBooks();
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 0)" << std::endl;

    // Test 16: Modify Non-Existent Order
    std::cout << "\n--- TEST 16: Modify Non-Existent Order ---" << std::endl;
    book.clearBooks();
    book.add_order(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    book.modify_order(true, 2, 60);
    std::cout << "Number of Bids: " << book.getNumberOfBids() << " (Expected: 1)" << std::endl;

    // Test 17: Invalid Quantity Modification
    std::cout << "\n--- TEST 17: Invalid Quantity Modification ---" << std::endl;
    book.add_order(1622505600, true, 1, 100, 50, "NYSE", "AAPL", LIMIT);
    book.modify_order(true, 1, -20);
    std::cout << "Modified Order Quantity: " << book.getOrderBids(0)->getQuantity() << " (Expected: 50)" << std::endl;

    // Test 18: Get Venue of Bid
    std::cout << "\n--- TEST 18: Get Venue of Bid ---" << std::endl;
    std::cout << "Venue of First Bid: " << book.getOrderBids(0)->getVenue() << " (Expected: NYSE)" << std::endl;

    // Test 20: Get Order ID of Bid
    std::cout << "\n--- TEST 20: Get Order ID of Bid ---" << std::endl;
    std::cout << "Order ID of First Bid: " << book.getOrderBids(0)->getID() << " (Expected: 1)" << std::endl;

    return 0;
}
