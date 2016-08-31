all:
	g++ -std=c++14 *.h *.cpp -o Pricer

run:
	./Pricer feed.txt

clean:
	rm -f *.gch *~