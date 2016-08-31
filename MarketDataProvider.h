#ifndef MARKETDATAPROVIDER_H_
#define MARKETDATAPROVIDER_H_

#include <string>
#include <vector>
#include "Exceptions.h"

namespace trading {

/**
 * Market Data Provider as Meyers' Singleton
 */
class MarketDataProvider{
public:

	// Read trades from market data file
	void readMarketDataFile(const std::string& filename);

	// Not EOF?
	bool hasNextMessage();

	// Get next message from the market data file
	const std::string& nextMessage();

private:
	std::vector<std::string> readFile2Vector(const std::string& filename);
	
	std::string filename_;
	std::vector<std::string> messages_;
	std::vector<std::string>::iterator cur_;
};

} // end of namespace


inline bool trading::MarketDataProvider::hasNextMessage() {
	return (cur_ != messages_.end());
}


inline const std::string& trading::MarketDataProvider::nextMessage() {
	if (hasNextMessage()) {
		std::vector<std::string>::iterator saved = cur_;
		cur_++;
		//std::cout << "saved = " << *saved << std::endl;
		return *saved;
	} else {
		throw(trading::OutOfBounds());
	}
}

#endif /* MARKETDATAPROVIDER_H_ */