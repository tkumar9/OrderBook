#include <iostream>
#include <iomanip>  // setprecision
#include <cassert>
#include <string>   // getline
#include <typeinfo>
#include <thread>

#include "OrderBook.h"
#include "MarketDataProvider.h"
#include "MarketOrder.h"
#include "Parser.h"
#include "Exceptions.h"

void submitOrder(trading::Parser parser, const std::string& msg)
{
	// Parse message
	trading::MarketOrder order;
	try {		
		order = parser.parse(msg);

	} catch (const trading::ParseException&) {
		std::cout << "Skipping this message due to parsing errors: " << msg<< std::endl;
		throw;
	}
	
	// Submit the message to the order book
	try {
		trading::OrderBook::getInstance().processOrder(order);
	} catch (const trading::OrderBookException&) {
		std::cout << "Error in order book when submitting the following order: " << order.toString()<<std::endl;
		throw;
	}
}

void runOrdersFile(std::string feedFile)
{
	std::cout<<"Thread id: "<<std::this_thread::get_id()<<std::endl;
	
	trading::MarketDataProvider dataProvider;
	try {
			dataProvider.readMarketDataFile(feedFile);
			
		} catch (const trading::BadMarketDataFile& e) {
			std::cout << "Error opening the market data file"<<std::endl;
			abort();
		}	
	
	trading::Parser parser;
	// Main loop
	while (true) {
		// break loop on EOF if using a market data file
		if (!dataProvider.hasNextMessage()) {
			break;
		}		
		auto msg = dataProvider.nextMessage();
		std::cout << msg << std::endl;
		
		try{
		submitOrder(parser, msg);		
		}
		catch (const trading::Exception& e) { // exception catch-all
			std::cout << "Exception: " << typeid(e).name()<<std::endl;
		}
	}
}

int main(int argc, char* argv[]) {
	try {
		std::cout << std::setiosflags(std::ios::fixed);
		
		std::cout << "Starting Trading Simulator"<<std::endl;
		
		//Test case for multithreading (reading order feeds and submit orders in threads to single order book)
		if(argc > 1)
		{
			std::vector<std::thread> mythreads;
			for(auto i = 1; i < argc; i++)
			{				
				mythreads.push_back(std::thread(runOrdersFile, argv[i]));
			}		

			for(auto& th : mythreads)
				th.join();
		}
		//Test case for cmd line input orders
		else
		{	
			trading::Parser parser;
			// Main loop
			while (true) {			
				std::string msg;
				// "Receive" new message
				std::getline(std::cin, msg);
				// std::cout << msg << std::endl;
				submitOrder(parser, msg);
			}
		}
		// Done
		
		std::string result;
		try {
			result = trading::OrderBook::getInstance().printBook();
			std::cout<< std::endl << "Result: " << result<<std::endl;
		} catch (const trading::OrderBookException&) {
			std::cout << "Error while printing book";
			throw;
		}
	
		std::cout << "Simulator is stopped.";
		
	} catch (const trading::Exception& e) { // exception catch-all
		std::cout << "Exception: " << typeid(e).name()<<std::endl;
	}
	return 0;
}
		
		
			