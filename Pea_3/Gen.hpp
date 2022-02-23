#pragma once
#include <vector>
#include <iostream>
#include <time.h>
#include <algorithm>
#include <iomanip>
#include <random>
#include <Windows.h>
#include "Instance.hpp"
using namespace std;

class Instance;

class Gen {
public:
	Gen(Instance* instanceTmp, bool showMessages);
	pair<int, double> startAlg();
	void generatePopulation();
	int countValue(vector<int>* visitedTemp);
	void tournamentSelection();
	void displayPopulation();
	pair<int, int> randomPaths();
	void orderedCrossover();
	void inversionMutation();
	void printBestResult();
	void checkPopulation();
	void printFinal();
	long long int read_QPC();

private:
	Instance* instance = nullptr;
	vector <pair<vector<int>, int>> population;
	int popSize;
	pair<vector<int>, int> bestResult;
	int iterator;
	double timeBest;
	long long int frequency, start, elapsed;
	int valueStart;
	int notBetter;

	double bestTime;
	double mutationProb;
	double crossoverFactor;
	bool show;
};