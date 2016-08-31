#ifndef MARKETORDER_H_
#define MARKETORDER_H_

#include <iostream>
#include <string>
#include <sstream>
#include "Trader.h"

namespace trading {

enum OrderSide {buy, sell};

// Market Order
struct MarketOrder {
	std::string toString() const;

	// We'll use these typedefs in the OrderBook class
	typedef unsigned long int Size;  // Order size

	long id;
	std::shared_ptr<trading::Trader> trader;
	std::string stock;
	OrderSide side;               // order side
	Size size;       			  // order size (# shares)
};

} // end of namespace	


// Definitions of inline functions

inline std::string trading::MarketOrder::toString() const {
	std::stringstream oss;

	oss << "Order: id " << id << " Trader= " << trader->name << " Stock= " << stock << " Side= " << (side == buy ? "B" : "S") <<" Size= " << size;
	return oss.str();
}

#endif /* MARKETORDER_H_ */