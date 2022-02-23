#include "Gen.hpp"
using namespace std;

Gen::Gen(Instance* instanceTmp, bool showMessages) {
	instance = instanceTmp;
	popSize = instanceTmp->getCitiesAmount();
	vector<int> empty;
	bestResult = make_pair(empty, INT_MAX);
	iterator = 0;
	notBetter = 0;
	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);

	show = showMessages;
	mutationProb = 0.01;
}

pair<int, double> Gen::startAlg() {
	generatePopulation();

	start = read_QPC();
	while (notBetter < popSize) {
		tournamentSelection();
		orderedCrossover();
		inversionMutation();
		checkPopulation();
		iterator++;
	}
	if(show) printFinal();

	return make_pair(bestResult.second, bestTime);
}

void Gen::generatePopulation() {
	vector<int> path;
	auto rng = default_random_engine{};

	for (int i = 0; i < instance->getCitiesAmount(); i++)
		path.push_back(i);

	for (int i = 0; i < popSize; i++) {
		shuffle(begin(path), end(path), rng);
		population.push_back(make_pair(path, countValue(&path)));
	}
}

void Gen::inversionMutation(){
	for (int len = 0; len < popSize; len++) {

		double chance = rand() / RAND_MAX;
		if (chance < mutationProb) {

			int index = rand() % population.size();
			int pos1 = rand() % instance->getCitiesAmount();
			int pos2;

			vector<int> chosen = population.at(index).first;
			vector<int> mutated;

			do {
				pos2 = rand() % instance->getCitiesAmount();
			} while (pos1 == pos2);

			if (pos1 > pos2) {
				int temp = pos1;
				pos1 = pos2;
				pos2 = temp;
			}

			for (int i = 0; i < pos1; i++)
				mutated.push_back(chosen.at(i));
			for (int i = pos2; i >= pos1; i--)
				mutated.push_back(chosen.at(i));
			for (int i = pos2 + 1; i < instance->getCitiesAmount(); i++)
				mutated.push_back(chosen.at(i));

			population.push_back(make_pair(mutated, countValue(&mutated)));
		}
	}
}

void Gen::printBestResult() {
	double PRD = (bestResult.second - instance->getOptimalHamiltonValue()) / (double)instance->getOptimalHamiltonValue() * 100.0;
	cout << setw(4) << iterator << " " << setw(6) << bestResult.second << " " << fixed << setprecision(2) << setw(7) << PRD << "%";
	cout << " | Czas [ms] = " << setprecision(0) << bestTime << endl;
}

void Gen::tournamentSelection() {
	int am = population.size() - popSize;
	for (int i = 0; i < am; i++) {
		pair<int, int> selected = randomPaths();
		if (population.at(selected.first).second > population.at(selected.second).second) 
			 population.erase(begin(population) + selected.first);
		else population.erase(begin(population) + selected.second);
	}
}

void Gen::orderedCrossover(){
	for (int len = 0; len < popSize; len++) {
		pair<int, int> toCrossover = randomPaths();
		int pos1 = rand() % instance->getCitiesAmount();
		int pos2;

		vector<int> first = population.at(toCrossover.first).first;
		vector<int> second = population.at(toCrossover.second).first;

		vector<int> crossoveredFirst;
		vector<int> crossoveredSecond;

		do {
			pos2 = rand() % instance->getCitiesAmount();
		} while (pos1 == pos2);

		if (pos1 > pos2) {
			int temp = pos1;
			pos1 = pos2;
			pos2 = temp;
		}

		int* t1 = new int[instance->getCitiesAmount()];
		int* t2 = new int[instance->getCitiesAmount()];

		for (int i = 0; i < instance->getCitiesAmount(); i++) {
			if (i >= pos1 && i <= pos2) {
				t1[i] = first.at(i);
				t2[i] = second.at(i);
			}
			else {
				t1[i] = -1;
				t2[i] = -1;
			}
		}

		for (int i = 0; i < instance->getCitiesAmount(); i++) {
			if (!(i >= pos1 && i <= pos2)) {

				while (true) {
					bool isIn = false;
					for (int j = pos1; j <= pos2; j++) {
						if (second[0] == t1[j]) {
							isIn = true;
							break;
						}
					}
					if (isIn) second.erase(begin(second));
					else {
						t1[i] = second[0];
						second.erase(begin(second));
						break;
					}
				}

				while (true) {
					bool isIn = false;
					for (int j = pos1; j <= pos2; j++) {
						if (first[0] == t2[j]) {
							isIn = true;
							break;
						}
					}
					if (isIn) first.erase(begin(first));
					else {
						t2[i] = first[0];
						first.erase(begin(first));
						break;
					}
				}
			}
		}

		for (int i = 0; i < instance->getCitiesAmount(); i++) {
			crossoveredFirst.push_back(t1[i]);
			crossoveredSecond.push_back(t2[i]);
		}

		delete(t1);
		delete(t2);

		population.push_back(make_pair(crossoveredFirst, countValue(&crossoveredFirst)));
		population.push_back(make_pair(crossoveredSecond, countValue(&crossoveredSecond)));
	}
}

void Gen::printFinal() {
	double PRD = (valueStart - instance->getOptimalHamiltonValue()) / (double)instance->getOptimalHamiltonValue() * 100.0;
	cout << setw(4) << "0" << " " << setw(6) << valueStart << " " << fixed << setprecision(2) << setw(7) << PRD << "%" << endl;

	for (int city : bestResult.first) cout << city << " -> ";
	cout << bestResult.first.at(0) << endl;
}

pair<int, int> Gen::randomPaths() {
	int i = rand() % population.size();
	int j;
	do {
		j = rand() % population.size();
	} while (i == j);
	return make_pair(i, j);
}

int Gen::countValue(vector<int>* visitedTemp) {
	int result = 0;
	int last = 0;
	int tempSize = visitedTemp->size();

	for (int i = 1; i < tempSize; last = i, i++)
		result += instance->getWeightMatrix()[visitedTemp->at(last)][visitedTemp->at(i)];
	result += instance->getWeightMatrix()[visitedTemp->at(0)][visitedTemp->at(tempSize - 1)];

	return result;
}

void Gen::displayPopulation() {
	int s = population.size();
	for (int i = 0; i < s; i++) {
		int s2 = population.at(i).first.size();
		for (int j = 0; j < s2; j++) {
			cout << population.at(i).first.at(j) << " -> ";
		}
		cout << population.at(i).first.at(0) << " [" << population.at(i).second << "]" << "\n";
	}
	cout << "==============================\n";
}

void Gen::checkPopulation() {
	bool writeOut = false;
	int s = population.size();
	elapsed = read_QPC() - start;
	for (int i = 0; i < s; i++) {
		if (bestResult.second > population.at(i).second) {
			bestResult = population.at(i);
			bestTime = (1000.0 * elapsed) / frequency;
			if (iterator == 0) valueStart = bestResult.second;
			writeOut = true;
		}
	}
	if (writeOut) {
		if(show) printBestResult();
		writeOut = false;
		notBetter = 0;
	}
	else notBetter++;
}

long long int Gen::read_QPC()
{
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	return((long long int)count.QuadPart);
}