#ifndef ORDERBOOK_H_
#define ORDERBOOK_H_

#include <unordered_map>
#include <string>
#include <sstream>
#include <algorithm>
#include <cassert>
#include <mutex>
#include <shared_mutex>
#include<vector>

#include "Exceptions.h"
#include "MarketOrder.h"

namespace trading {

struct SynchVector
{
	SynchVector():mtx(new std::mutex)
	{}
    std::shared_ptr<std::mutex> mtx;
	OrderSide openSide;
    std::vector<trading::MarketOrder> orders;
};

/**
 * Order Book
 */
class OrderBook {
public:
	// Singleton
	static OrderBook& getInstance();

	// Print order book
	std::string printBook();

	// Process a new market order
	void processOrder(trading::MarketOrder& order);
	void executeOrder(trading::SynchVector& vec, trading::MarketOrder& order);

private:
	mutable std::shared_timed_mutex bookMtx;
	
	// Multimaps: stock -> order list
	//For each stock, it has seperate vector. As we are not considering stock price (as problem stated), only one container is needed 
	//as at a time only one side (but/sell) order will be existed in order list.
	//Used wraper on vector for locking it on writing to the same stock by multiple threads
	//for better performance, we can use node based synchronised list (node based locking) but implementing that will be more complex, so I used vector locking for the simplicity.
	std::unordered_map<std::string, trading::SynchVector> ordersMap;


// Singleton stuff
private:
	OrderBook() { };
	OrderBook(OrderBook const&);      // Don't Implement
	void operator=(OrderBook const&); // Don't implement

};

} // end of namespace


// Definitions of inline functions
inline trading::OrderBook& trading::OrderBook::getInstance() {
	static OrderBook _instance; // Guaranteed to be destroyed. Instantiated on first use.
	return _instance;
}


#endif /* ORDERBOOK_H_ */