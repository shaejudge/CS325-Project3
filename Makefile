CFLAGS = -Wall -fpic -coverage -lm

tsp: tsp.cpp
	g++ -o tsp tsp.cpp $(CFLAGS)