#pragma once
#include <string>
#include <vector>


class Instance {
public:
	Instance(std::string instanceName, int citiesAmount, int** weightMatrix, int optimalHamiltonValue);
	std::string getInstanceName();
	int getCitiesAmount();
	int** getWeightMatrix();
	int getOptimalHamiltonValue();
	void displayInformations();

private:
	std::string instanceName;
	int citiesAmount;
	int** weightMatrix;
	int optimalHamiltonValue;
};