#ifndef PARSER_H_
#define PARSER_H_

#include <sstream>
#include <iostream>
#include <vector>
#include <cstdlib> //atol
#include <atomic>

#include "MarketOrder.h"
#include "Exceptions.h"
#include "Trader.h"

namespace trading {

class Parser {
public:
	// Basic Market Order Parser
	std::vector<std::string> tokenize(const std::string& str, const char& delimiter);
	MarketOrder parse(const std::string& msg);	
	
	static std::atomic<int> orderId;
};

std::atomic<int> Parser::orderId(0);
} // end of namespace

// Definitions of inline functions
inline std::vector<std::string> trading::Parser::tokenize(const std::string& str, const char& delimiter) {
	std::vector<std::string> fields;
	std::istringstream is(str);
	std::string s;
	while (std::getline(is, s, delimiter)) {
		fields.push_back(s);
	}
	return fields;
}
	
inline trading::MarketOrder trading::Parser::parse(const std::string& msg) {
	// Get fields
	std::vector<std::string> fields = tokenize(msg, ' ');

	// Basic check
	if (fields.size() != 4) {
		throw BadParse();
	}

	trading::MarketOrder order;

	// Parse message	
	// "order": A S B 100
	order.id = ++orderId;
	
    std::string traderName = fields.at(0);	
	order.trader = trading::TraderManager::getInstance().getTrader(traderName);
	order.stock = fields.at(1);

	// Order side
	std::string orderSide = fields.at(2);
	if (orderSide.compare("B") == 0) {
		order.side = buy;
	} else if (orderSide.compare("S") == 0) {
		order.side = sell;
	} else {
		throw BadParse();
	}

	order.size = std::atol(fields.at(3).c_str());
	

	std::cout << "Parsed order: " << order.toString()<<std::endl;
	return order;
}

#endif /* PARSER_H_ */