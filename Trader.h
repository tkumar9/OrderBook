#ifndef Trader_H_
#define Trader_H_

#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <utility>      // std::pair

namespace trading {

class Trader{
public:

    Trader(std::string n):name(n)
	{}

     //callback functions
     void notify(const long orderId)
	 {
	     std::cout << "Trader notified " << name << " Order executed Id: "<< orderId << std::endl;
	 }
	 
	 std::string name;
};


// Trader Manager
class TraderManager{	
public:
     // Singleton
	static TraderManager& getInstance()
	{
		static TraderManager _instance; // Guaranteed to be destroyed. Instantiated on first use.
		return _instance;
	}
	
     std::shared_ptr<Trader> getTrader(std::string name)
	 {
		auto tr = std::make_shared<Trader>(name);
	    auto pair_elem = trader_map.insert(std::make_pair(name, tr));
		return pair_elem.first->second;		
	 }
private:     
	std::unordered_map<std::string, std::shared_ptr<Trader> > trader_map;
	
	// Singleton stuff
	TraderManager() { }
	TraderManager(TraderManager const&); // Don't Implement
	void operator=(TraderManager const&);     // Don't implement
};

} // end of namespace

#endif /* Trader_H_ */