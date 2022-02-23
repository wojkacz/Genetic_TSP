#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <iomanip>
#include "Instance.hpp"
#include "Gen.hpp"
using namespace std;

Instance* createInstanceByFileName(string fileName);
Instance* askForFile();
void showMenu(Instance* instance);

int main() {
	srand((unsigned int)time(NULL));
	showMenu(askForFile());
	return 0;
}

// Funkcja pytająca o nazwe pliku, działa do czasu podania poprawnej wartości - takiej z której da się stworzyć instancje
Instance* askForFile() {
	while (true) {
		string fileName;
		cout << "Podaj nazwe pliku [z rozszerzeniem]: ";
		cin >> fileName;

		Instance* instance = createInstanceByFileName(fileName);

		if (instance == nullptr) {
			cout << "Nie udalo sie stworzyc instancji z podanego pliku. Sprobuj ponownie.\n=============================================\n" << endl;
		}
		else return instance;
	}
}

// Funkcja wczytująca dane z pliku i tworząca z niej instancje
Instance* createInstanceByFileName(string fileName) {
	ifstream file;
	string instanceName;
	int citiesAmount;
	int** weightMatrix;
	int optimalHamiltonValue;

	file.open(fileName);
	if (file.is_open()) {

		// Wczytywanie nazwy instancji
		file >> instanceName;
		if (file.fail()) {
			cout << "Wczytywanie przerwane - blad przy odczycie nazwy instancji!" << endl;
			file.close();
			return nullptr;
		}

		// Wczytywanie ilości miast
		file >> citiesAmount;
		if (file.fail()) {
			cout << "Wczytywanie przerwane - blad przy odczycie ilosci miast!" << endl;
			file.close();
			return nullptr;
		}

		// Wczytywanie pełnej macierzy wag krawędzi
		weightMatrix = new int* [citiesAmount];
		for (int i = 0; i < citiesAmount; i++) {
			weightMatrix[i] = new int[citiesAmount];
			for (int j = 0; j < citiesAmount; j++) {
				file >> weightMatrix[i][j];
				if (file.fail()) {
					cout << "Wczytywanie przerwane - blad przy odczycie macierzy wag krawedzi!" << endl;
					file.close();
					return nullptr;
				}
			}
		}

		// Wczytywanie optymalnej wartosci Hamiltona
		file >> optimalHamiltonValue;
		if (file.fail()) {
			cout << "Wczytywanie przerwane - blad przy odczycie optymalnej wartosci Hamiltona!" << endl;
			file.close();
			return nullptr;
		}

		file.close();
		return new Instance(instanceName, citiesAmount, weightMatrix, optimalHamiltonValue);
	}

	else {
		cout << "Wczytywanie przerwane - nie udalo sie otworzyc pliku!" << endl;
		return nullptr;
	}
}

// Funkcja wyświetlająca i obsługująca menu w konsoli
void showMenu(Instance* instance) {

	// Zmienna wykorzystywana do obsługi menu
	string selected;
	Gen gen = Gen(instance, true);
	pair<int, double> resultGen;
	int sumOfLength = 0;
	double sumOfTime = 0;

	while (true) {
		do {
			system("cls");
			cout << "=============================================" << endl;
			cout << "Instancja: " << instance->getInstanceName() << endl;
			cout << "1. Wyswietl pelne informacje o instancji" << endl;
			cout << "2. Wykonaj alorytm genetyczny" << endl;
			cout << "3. Wykonaj testy i wyswietl srednie" << endl;
			cout << "4. Wczytaj inna instancje" << endl;
			cout << "5. Opusc program" << endl;
			cout << "Wpisz liczbe calkowita odpowiadajaca wybranej opcji." << endl;
			cout << "Wybor >> ";
			cin >> selected;
		} while (selected != "1" && selected != "2" && selected != "3" && selected != "4" && selected != "5");

		switch (stoi(selected)) {
		case 1: // Informacje
			system("cls");
			instance->displayInformations();
			break;

		case 2: 
			system("cls");
			cout << "=============================================" << endl;
			cout << "Algorytm Genetyczny" << endl;

			gen = Gen(instance, true);
			gen.startAlg();

			system("pause");
			break;

		case 3: 
			system("cls");
			cout << "=============================================" << endl;
			cout << "Algorytm Genetyczny - Testy" << endl;
			sumOfLength = 0;
			sumOfTime = 0;

			for (int d = 0; d < 10; d++) {
				gen = Gen(instance, false);
				resultGen = gen.startAlg();
				sumOfLength += resultGen.first;
				sumOfTime += resultGen.second;
				cout << "[Test " << d << "] Wynik: " << resultGen.first << "; Czas: " << resultGen.second << endl;
			}

			cout << "Optymalna wartosc: " << instance->getOptimalHamiltonValue() << endl;
			cout << "Ilosc miast: " << instance->getCitiesAmount() << endl;
			cout << "Srednie na podstawie 10 testow: " << endl;
			cout << "Srednia droga: " << (sumOfLength / 10) << endl;
			cout << "Sredni czas [ms] = " << (sumOfTime / 10.0) << endl;

			system("pause");
			system("pause");
			break;

		case 4: // Zmiana instancji
			system("cls");
			free(instance);
			instance = askForFile();
			break;

		case 5: // Wyjscie
			return;
		}
	}
}