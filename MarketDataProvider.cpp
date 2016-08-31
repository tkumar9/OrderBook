#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm> // copy

#include "MarketDataProvider.h"

// Read file to a vector of strings (each one is a line)
std::vector<std::string> trading::MarketDataProvider::readFile2Vector(const std::string& filename) {
	std::cout<< "Opening file " << filename.c_str()<<std::endl;

	std::ifstream is(filename.c_str());

	if (!is) {
		throw trading::BadMarketDataFile();
	}

	std::string str;
	std::vector<std::string> lines;

	while (!is.eof()) {
		std::getline(is, str);
		lines.push_back(str);
	}

	is.close();
	std::cout<< "Number of lines read = " << lines.size()<<std::endl;

	return lines;
}

void trading::MarketDataProvider::readMarketDataFile(const std::string& filename) {
	messages_.clear();
	filename_ = filename;
	messages_ = readFile2Vector(filename_);
	std::cout << "MarketDataProvider is initialized with " << messages_.size() << " messages"<<std::endl;
	// std::copy(messages.begin(), messages.end(), std::ostream_iterator<std::string>(std::cout,"\n"));
	cur_ = trading::MarketDataProvider::messages_.begin();
}