# OrderBook
epam order book


Compile command: 
g++ -std=c++14 *.h *.cpp -o Pricer 

Usage: we can run it either with no argument (take input message in command line) or with arguments as feed files (contains order messages and each file will be processed in seperate thread). 

./Pricer

or 

./Pricer feed1.txt

or 

./Pricer feed1.txt feed4.txt

Message format: 
    Trader Stock Side Size 
    
    example: 
      Tarun CS B 200
      Alex CS S 200
