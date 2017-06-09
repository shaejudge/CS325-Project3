#include <iostream>
#include <cstdlib>
#include <fstream>
#include <cstring>
#include <string>
#include <time.h>
#include <math.h>

using namespace std;

struct City{
	int id;
	int xCoord;
	int yCoord;
	int edgevalue;
	struct City* next = NULL;
	struct City* prev = NULL;
};

struct cityLList{
	int size;
	struct City* frontSent;
	struct City* backSent;
};


//initialize the list of cities
static void initList(struct cityLList* list) {
	list->frontSent = new City;
	list->backSent = new City;
	
	list->frontSent->next = list->backSent;
	list->backSent->prev = list->frontSent;
	
	//initialize with an empty list
	list->size = 0;
}

//build the list of cities
struct City* randomStart(struct cityLList* list) {
	struct City* randomCity;
	srand(time(NULL));
	int randomNum = rand() % list->size;

	randomCity = list->frontSent->next;
	for (int i = 0; i < randomNum; i++)
		randomCity = randomCity->next;

	return randomCity;
}

void addToList(struct cityLList* list, struct City* city) {
	if (list->frontSent->next == list->backSent){
		city->next = list->backSent;
		city->prev = list->frontSent;
		
		list->frontSent->next = city;
		list->backSent->prev = city;
		
		list->size++;
	} else {
		city->next = list->backSent;
		city->prev = list->backSent->prev;
		
		list->backSent->prev->next = city;
		list->backSent->prev = city;
		
		list->size++;
	}
}

void removeFromList(struct cityLList* list, struct City* city) {
	city->prev->next = city->next;
	city->next->prev = city->prev;
	list->size--;
}

//measure distance from city to city
int findDistance(struct City* city1, struct City* city2) {
	int xdistance = city1->xCoord - city2->xCoord;
	int ydistance = city1->yCoord - city2->yCoord;
	int xdistancesquared = xdistance * xdistance;
	int ydistancesquared = ydistance * ydistance;
	int sum = xdistancesquared + ydistancesquared;
	int distance = round(sqrt(sum));
	return distance;
}

int findTotalCostOfTrip(struct cityLList* list) {
	struct City* ptr;
	ptr = list->frontSent->next;
	int edgeSum = 0;

	while (ptr != list->backSent){
		edgeSum += ptr->edgevalue;
		ptr = ptr->next;
	}

	return edgeSum;
}

void outputToFile(struct cityLList* list, string filename) {
	struct City* ptr = list->frontSent->next;
	int totalDistance = findTotalCostOfTrip(list);

	ofstream outFile;
	filename = filename + ".tour";
	outFile.open(filename.c_str());

	outFile << totalDistance << endl;
	for (int i = 0; i < list->size; i++){
		outFile << ptr->id << endl;
		ptr = ptr->next;
	}

	outFile.close();   
}

void printOutput(struct cityLList* list) {
	struct City* ptr = list->frontSent->next;;
	int totalDistance = findTotalCostOfTrip(list);

	cout << totalDistance << endl;
	for (int i = 0; i < list->size; i++){
		cout << ptr->id << endl;
		ptr = ptr->next;
	}

}

struct City* findNearestNeighbor(struct cityLList* nyvlist, struct City* city) {
	struct City* nearestNeighbor = city;
	int smallest = 100000;
	int distance;

	struct City* ptr = nyvlist->frontSent->next;
	while (ptr != nyvlist->backSent){
		distance = findDistance(ptr, city);
		
		if (distance < smallest){
			smallest = distance;
			nearestNeighbor = ptr;
			nearestNeighbor->edgevalue = smallest;
		}
		
		ptr = ptr->next;
	}
	return nearestNeighbor;
}

void buildPath(struct cityLList* vlist, struct City* city, struct cityLList* nyvlist) {
	struct City* startingCity = city;
	struct City* currentCity = city;
	struct City* nearestNeighbor;

	removeFromList(nyvlist, startingCity); 

	while (nyvlist->frontSent->next != nyvlist->backSent) {
		nearestNeighbor = findNearestNeighbor(nyvlist, currentCity);
		removeFromList(nyvlist, nearestNeighbor);
		addToList(vlist, nearestNeighbor);
		currentCity = nearestNeighbor;
	}

	//add starting city to visited
	int finalDistance = findDistance(currentCity, startingCity);
	startingCity->edgevalue = finalDistance;
	addToList(vlist, startingCity);

}


int main(int argc, char *argv[]) {
	clock_t start; 
	double duration; 
	struct cityLList* visitedCities = new cityLList;
	struct cityLList* nyVisitedCities = new cityLList;
	struct City* startCity = new City;

	initList(visitedCities);
	initList(nyVisitedCities);

	//file input
	City city[16000];
	int i = 0;
	string filename;
	fstream myfile(argv[1]);
	filename = argv[1];

	if (argv == 0) {
		cout << "Need Input";
	}
	
	//Create array of city structs
	while (myfile >> city[i].id >> city[i].xCoord >> city[i].yCoord){
		city[i].edgevalue = 0;
		addToList(nyVisitedCities, &city[i]);
		i = i + 1;
	}

	myfile.close();
	//end input

	start = clock(); 
	startCity = randomStart(nyVisitedCities);

	buildPath(visitedCities, startCity, nyVisitedCities);

	duration = (clock() - start) / (double)CLOCKS_PER_SEC; 
	printOutput(visitedCities);
    outputToFile(visitedCities, filename);
	cout << "Time: " << duration << endl; 

	return 0;
}