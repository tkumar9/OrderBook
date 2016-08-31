#include <utility>      // std::pair
#include "OrderBook.h"

void trading::OrderBook::executeOrder(trading::SynchVector& vec, trading::MarketOrder& order)
{	
	std::lock_guard<std::mutex> lock(*(vec.mtx));
	
	if(order.side == vec.openSide)
	{
		vec.orders.push_back(std::move(order));			
	}
	else
	{
		for(auto it=vec.orders.begin(); it != vec.orders.end(); /*++it*/)
		{
			if(it->size > order.size)
			{
				it->size -= order.size;
				order.trader->notify(order.id);
				break;
			}
			else if(it->size == order.size)
			{
				it->trader->notify(it->id);
				order.trader->notify(order.id);
				order.size -= it->size;
				vec.orders.erase(it);				
				break;
			}
			else
			{
				it->trader->notify(it->id);					
				order.size -= it->size;
				it = vec.orders.erase(it);
			}
		}
		
		if(vec.orders.empty() && order.size>0)
		{				
			vec.openSide = order.side;
			vec.orders.push_back(std::move(order));
		}			
	}		
}

void trading::OrderBook::processOrder(trading::MarketOrder& order) {
	
   std::unordered_map<std::string, trading::SynchVector>::iterator it;
   {
	  //using Shared mutex (r/w lock) for syhching reader/writer threads on unordered map
	  //Multiple reader can access map at the same time using shared lock but writer will have exclusive lock on shared mutex
      std::shared_lock<std::shared_timed_mutex> lock{bookMtx};
	  it = ordersMap.find(order.stock);
   }
   // if stock is not in Order Book
	if(it == ordersMap.end())
	{   
		//Writer will have exclusive lock on shared mutex
		std::unique_lock<std::shared_timed_mutex> lock{bookMtx};		

		//Double check lock - for synch threads processing first order of same stock
		it = ordersMap.find(order.stock);
		if(it  == ordersMap.end())
		{			
			std::cout << "Adding stock entry to the order book: " << order.stock <<std::endl;
			trading::SynchVector vec;	
			//std::lock_guard<std::mutex> lock{*(vec.mtx)};
			auto pairElem = ordersMap.insert(std::make_pair(order.stock, vec));
			auto&  mapIt = pairElem.first;
			mapIt->second.openSide = order.side;
			mapIt->second.orders.push_back(std::move(order));		
		}
		else
		{
			auto& vec = it->second;
			executeOrder(vec, order);
		}
	}
	else
	{
		auto& vec = it->second;
		executeOrder(vec, order);
	}
	
}

std::string trading::OrderBook::printBook() {
	std::stringstream oss;
	trading::MarketOrder order;

	oss << std::endl ;
	oss << "Order book:" << std::endl;	

	for (auto it = ordersMap.begin(); it != ordersMap.end(); it++) {
		auto orderVec = it->second.orders;
		if(!orderVec.empty())
			oss<<"\t Stock"<<std::endl;
		for(auto order: orderVec)
			oss << "\t\t" << order.toString() << std::endl;
	}

	oss << std::endl;
	return oss.str();
}