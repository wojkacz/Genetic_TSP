#include <iostream>
#include <iomanip>
#include "Instance.hpp"
using namespace std;

Instance::Instance(string instanceName, int citiesAmount, int** weightMatrix, int optimalHamiltonValue) {
	this->instanceName = instanceName;
	this->citiesAmount = citiesAmount;
	this->weightMatrix = weightMatrix;
	this->optimalHamiltonValue = optimalHamiltonValue;
}

string Instance::getInstanceName() {
	return instanceName;
}

int Instance::getCitiesAmount() {
	return citiesAmount;
}

int** Instance::getWeightMatrix() {
	return weightMatrix;
}

int Instance::getOptimalHamiltonValue() {
	return optimalHamiltonValue;
}

void Instance::displayInformations() {
	std::cout << "Nazwa instancji: " << instanceName << "\nIlosc miast: " << citiesAmount << "\nOptymalna wartosc Hamiltona: " << optimalHamiltonValue << "\nPelna macierz wag krawedzi:\n";
	for (int i = 0; i < citiesAmount; i++) {
		for (int j = 0; j < citiesAmount; j++)
			std::cout << std::left << setw(4) << weightMatrix[i][j] << " ";
		std::cout << std::endl;
	}
	system("pause");
}