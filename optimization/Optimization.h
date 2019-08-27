#ifndef _OPTIMIZATION_H_
#define _OPTIMIZATION_H_

#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <vector>
#include <tuple>
#include <algorithm> 
#include "PreAction.h"
#include <chrono>
#include <bitset>

using namespace std;
using namespace chrono;

static bool sortByWorker(const tuple<int, tuple<int, int, int, int>, vector<string>>& i, const tuple<int, tuple<int, int, int, int>, vector<string>>& j) {
	return (get<0>(get<1>(i)) < get<0>(get<1>(j)));
}
static bool sortByBuilding(const tuple<int, tuple<int, int, int, int>, vector<string>>& i, const tuple<int, tuple<int, int, int, int>, vector<string>>& j) {
	return (get<1>(get<1>(i)) < get<1>(get<1>(j)));
}
static bool sortByUnit(const tuple<int, tuple<int, int, int, int>, vector<string>>& i, const tuple<int, tuple<int, int, int, int>, vector<string>>& j) {
	return (get<2>(get<1>(i)) < get<2>(get<1>(j)));
}
static bool sortByAfterSim(const tuple<int, tuple<int, int, int, int>, vector<string>>& i, const tuple<int, tuple<int, int, int, int>, vector<string>>& j) {
	return (get<3>(get<1>(i)) < get<3>(get<1>(j)));
}
static bool sortByRate(const tuple<int, tuple<int, int, int, int>, vector<string>>& i, const tuple<int, tuple<int, int, int, int>, vector<string>>& j) {
	return (get<0>(i) < get<0>(j));
}
class Optimization
{
public:
	Optimization(string u) : objectiveUnit(u) {
		allUnits = getUnitsFromTechTree(objectiveUnit);
		PreAction pA("protoss_techtree.csv");
		ProductionStruct = pA.getProductionStruct(objectiveUnit);
		//calculate the number of elites
		eliteNum = individual / 10;
		if ((eliteNum % 2) != 0)
		{
			eliteNum++;
		}
		//initialize random seed
		srand(time(NULL));
	};
	vector<tuple<string, string, string>> getUnitsFromTechTree(string unit) {
		vector<tuple<string, string, string>> aU;
		ifstream outfile(techTree, ifstream::in);
		string name = "";
		string producedBy = "";
		string dependency = "";
		string pass;
		getline(outfile, pass, '\n');
		getline(outfile, pass, '\n');
		while (!outfile.eof())//if has not been the end of the file
		{
			getline(outfile, name, ',');
			if (name != "#buildings")
			{
				if (name == unit)
				{
					getline(outfile, pass, ',');
					getline(outfile, pass, ',');
					vespene = stoi(pass);
					for (size_t i = 0; i < 6; i++)
					{
						getline(outfile, pass, ',');
					}
				}
				else
				{
					for (size_t i = 0; i < 8; i++)
					{
						getline(outfile, pass, ',');
					}
				}
				getline(outfile, producedBy, ',');
				getline(outfile, dependency, ',');
				aU.push_back(make_tuple(name, producedBy, dependency));
			}
			getline(outfile, pass, '\n');
		}
		outfile.close();
		return aU;
	}
	string randomUnit(vector<string>& canBeBuilded) {
		int length = canBeBuilded.size();
		string unit = canBeBuilded[rand() % length];
		return unit;
	}
	vector<string> formSkeleton(string dependency) {
		vector<string> skeleton;
		string d = "";
		if (dependency != supply)//"pylon" for protoss; "supply_depot" for terren; "hatchery" for zerg
		{
			for (tuple<string, string, string>& t : allUnits) {
				if (get<0>(t) == dependency) d = get<2>(t);
			}
			skeleton = formSkeleton(d);
			skeleton.push_back(dependency);
			return skeleton;
		}
		else
		{
			skeleton.push_back(supply);
			return skeleton;
		}
		skeleton.push_back(dependency);

	}
	vector<string> buildListGenerator() {
		refinery = 0;
		vector<string> canBeBuilt = { worker, supply, ProductionStruct, objectiveUnit };
		vector<string> baseUnits = { worker, supply };
		vector<string> withBuilding = { worker, supply, ProductionStruct };
		if (vespene > 0) {
			canBeBuilt.push_back(assimilator);
			baseUnits.push_back(assimilator);
			withBuilding.push_back(assimilator);
		}
		if (objectiveUnit == worker) canBeBuilt.push_back(baseCamp);//zerg ignore

																	//form the skeleton of the list
		string dependency = "";
		for (tuple<string, string, string>& relation : allUnits) {
			if (get<0>(relation) == objectiveUnit)
			{
				dependency = get<2>(relation);
			}
		}
		
		vector<string> buildList;
		if (dependency == "") buildList = formSkeleton(ProductionStruct);
		else buildList = formSkeleton(dependency);
		buildList.push_back(objectiveUnit);

		string unit = "";
		int length = rand() % 5 + 18;
		int availLength = length - buildList.size();
		for (size_t j = 0; j < availLength; j++)
		{
			int i = 0;
			int k = 0;
			for (i = 0; i < buildList.size(); i++)
			{
				if (buildList[i] == ProductionStruct) {
					break;
				}
			}
			if (dependency == "") k = i;
			else {
				for (k = 0; k < buildList.size(); k++)
				{
					if (buildList[k] == dependency) {
						break;
					}
				}
			}
			int position = rand() % (buildList.size() + 1);

			if (position <= i)
			{
				if (refinery >= 2)
				{
					do
					{
						unit = randomUnit(baseUnits);
					} while (unit == assimilator);
				}
				else unit = randomUnit(baseUnits);
				buildList.insert(buildList.begin() + position, unit);
			}
			else {
				if (refinery >= 2)
				{
					do
					{
						if (position > k) unit = randomUnit(canBeBuilt);
						else unit = randomUnit(withBuilding);
					} while (unit == assimilator);
				}
				else {
					if (position > k) unit = randomUnit(canBeBuilt);
					else unit = randomUnit(withBuilding);
				}
				if (position > k) {
					int probability = rand() % 10;
					if (probability < 4)buildList.insert(buildList.begin() + position, objectiveUnit);
					else buildList.insert(buildList.begin() + position, unit);
				}
				else {
					buildList.insert(buildList.begin() + position, unit);
				}
			}
			if (unit == assimilator) refinery++;
		}
		return buildList;
	}
	vector<vector<string>> firstGeneration() {
		vector<vector<string>> fG;
		for (size_t i = 0; i < individual; i++)
		{
			fG.push_back(buildListGenerator());
		}
		return fG;
	}
	vector<vector<string>> selection(vector<tuple<int, tuple<int, int, int, int>, vector<string>>>& r) {
		vector<vector<string>> elites;
		vector<tuple<int, tuple<int, int, int, int>, vector<string>>> results;
		PreAction pA(techTree);
		for (tuple<int, tuple<int, int, int, int>, vector<string>> ind : r) {
			vector<string> indi = get<2>(ind);
			if (pA.preCheck(indi)) {
				results.push_back(ind);
			}
		}
		eliteNum = results.size() / 10;
		int number = results.size() / 40;
		if ((eliteNum % 2) != 0)
		{
			eliteNum++;
		}
		//sort by workers and rate the result
		sort(results.begin(), results.end(), sortByWorker);
		for (int i = eliteNum; i > (eliteNum - number); i--) {
			get<0>(results[eliteNum - i]) += i;
		}

		//sort by production structure and rate the result
		sort(results.begin(), results.end(), sortByBuilding);
		for (int i = eliteNum; i > (eliteNum - number); i--) {
			get<0>(results[eliteNum - i]) += i;
		}

		//sort by after simulation and rate the result
		sort(results.begin(), results.end(), sortByAfterSim);
		for (int i = eliteNum; i > (eliteNum - number); i--) {
			get<0>(results[eliteNum - i]) += i;
		}
		//sort by unit number and rate the result
		sort(results.begin(), results.end(), sortByUnit);
		for (int i = eliteNum; i > 0; i--) {
			get<0>(results[eliteNum - i]) += i;
		}

		//sort by rate score
		sort(results.begin(), results.end(), sortByRate);
		int maximun = 0;
		for (int i = 0; i < eliteNum; i++) {
			elites.push_back(get<2>(results[i]));
			maximun = max(maximun, get<2>(get<1>(results[i])));
		}
		cout << "max number of objective units: " << maximun << endl;
		return elites;
	}

	vector<vector<string> > Reproduction(vector<vector<string> >& oldGeneration)
	{
		int N = oldGeneration.size(); // the total number of chromosomes

		vector<vector<string> > newGeneration;

		srand(time(0));
		while (oldGeneration.size()>1) {
			if (oldGeneration.size() > 3) {
				vector<vector<string> > couple;
				vector<int> indexS;

				// randomly pick 4 chromosomes
				//srand(time(0));
				while(indexS.size()<4||oldGeneration.size()<4) {
					int n = oldGeneration.size();
					int index = rand() % n;
					/*
					couple.push_back(oldGeneration[index]);
					swap(oldGeneration[index], oldGeneration[n - 1]);
					oldGeneration.pop_back();*/
					bool flag = true;
					for (int j = 0; j < indexS.size(); j++) {
						if (index == indexS[j]) { flag = false; break; }
						else {
							if (oldGeneration[index].size() == oldGeneration[indexS[j]].size()) {
								if (distance(oldGeneration[index], oldGeneration[indexS[j]]) == 0) {
									//delete the repeated buildlist from old Generation
									swap(oldGeneration[index], oldGeneration[oldGeneration.size() - 1]);
									oldGeneration.pop_back();
									flag = false;
									break;
								}
							}
						}
					}
					if (flag)  indexS.push_back(index); 
				}
				cout << "The size of oldGeneration is :" << oldGeneration.size() << endl;

				for (int i = 0; i < indexS.size(); i++) {
					couple.push_back(oldGeneration[indexS[i]]);
					cout << indexS[i] << endl;
				}

				//pick two chromosomes with biggest distance
				tuple<int, int> maxPair = distanceSelection(couple);
				cout << "get a pair" << endl;
				vector<string> father = couple[get<0>(maxPair)];
				vector<string> mother = couple[get<1>(maxPair)];

				//delete the pair from old generation
				swap(oldGeneration[get<0>(maxPair)], oldGeneration[oldGeneration.size() - 1]);
				oldGeneration.pop_back();
				swap(oldGeneration[get<1>(maxPair)], oldGeneration[oldGeneration.size() - 1]);
				oldGeneration.pop_back();

				auto start = system_clock::now();
				vector<vector<string> > babies = makeBabies(father, mother);
				auto end = system_clock::now();
				auto duration = duration_cast<microseconds>(end - start);

				cout << "Time costed for a pair:(Seconds)" << double(duration.count()) * microseconds::period::num / microseconds::period::den << endl;
				cout << "The size of this pair is: " << babies.size() << endl;
				//newGeneration.resize(newGeneration.size() + babies.size());
				newGeneration.insert(newGeneration.end(), babies.begin(), babies.end());

				//cout << "finished a pair" << endl;

				/*
				for (int i = 0; i < 4; i++)
				{
					if (i != get<0>(maxPair) && i != get<1>(maxPair))
						oldGeneration.push_back(couple[i]);
				}*/
			}
			else
				if (oldGeneration.size() > 1) {
					tuple<int, int> maxPair = distanceSelection(oldGeneration);

					vector<string> father = oldGeneration[get<0>(maxPair)];
					vector<string> mother = oldGeneration[get<1>(maxPair)];

					auto start = system_clock::now();
					vector<vector<string> > babies = makeBabies(father, mother);
					auto end = system_clock::now();
					auto duration = duration_cast<microseconds>(end - start);

					//delete the pair from old generation
					swap(oldGeneration[get<0>(maxPair)], oldGeneration[oldGeneration.size() - 1]);
					oldGeneration.pop_back();
					swap(oldGeneration[get<1>(maxPair)], oldGeneration[oldGeneration.size() - 1]);
					oldGeneration.pop_back();

					cout << "Time costed for a pair:(Seconds)" << double(duration.count()) * microseconds::period::num / microseconds::period::den << endl;
					cout << "The size of this pair is: " << babies.size() << endl;
					//newGeneration.resize(newGeneration.size() + babies.size());
					newGeneration.insert(newGeneration.end(), babies.begin(), babies.end());

					//cout << "finished a pair" << endl;
				}
		}

		return newGeneration;
	}

	tuple<int, int> distanceSelection(vector<vector<string> > couple)
	{
		int n = couple.size();

		int disMax = 0;
		int a = n - 1;
		int b = n - 2;
		int count = 0;
		for (int i = n - 1; i > 0; i--) {
			for (int j = i - 1; j > -1; j--) {
				int length = max(couple[i].size(), couple[j].size());
				vector<string> tempA = Extension(length, couple[i]);
				vector<string> tempB = Extension(length, couple[j]);
				int dis = distance(tempA, tempB);
				if (disMax < dis) {
					disMax = dis;
					a = i;
					b = j;
				}
				count++;
				cout << "round " << count <<" i= "<<i<<" j = "<<j<< endl;
			}
		}

		return make_tuple(a, b);
	}

	vector<string> Extension(int length, vector<string> temp) {
		//extend the buildlist to certain length
		//int n = temp.size();
		//PreAction pA(techTree);
		srand(time(0));
		vector<string> canBeBuilt = { worker, supply, ProductionStruct, objectiveUnit};  //todo: add-on
		while (temp.size() < length) {
			int index = rand() % canBeBuilt.size();
			temp.push_back(canBeBuilt[index]);
		}
		return temp;
	}

	int distance(vector<string> tempA, vector<string> tempB) {
		int length = tempA.size();
		int distance = 0;
		cout << "calculating distance for pair of length "<<length << endl;
		for (int i = 1; i <= length; i++) {
			if (tempA[i - 1] != tempB[i - 1])
				distance += length - i;
			//cout << i << endl;
		}
		return distance;
	}

	vector<vector<string> > makeBabies(vector<string> father, vector<string> mother) {
		int length = max(father.size(), mother.size());
		vector<string> fatherL = Extension(length, father);
		vector<string> motherL = Extension(length, mother);
		vector<vector<string> > babies;
		//Checker checker;
		//Techtree techtree;
		PreAction pA(techTree);

		cout << "begin make babies" << endl;
		/*
		for (int i = 0; i < length; i++)
		{
			if (babies.size() > 0) {
				if (fatherL[i] == motherL[i]) {
					for (int j = 0; j < babies.size(); j++)
					{
						
						if (checker.CheckDependency(fatherL[i], techtree, babies[j]) &&
						checker.CheckProducer(fatherL[i], techtree, babies[j]))
						babies[j].push_back(fatherL[i]);
						babies[j].push_back(fatherL[i]);
						if (pA.preCheck(babies[j])) {
						}
						else
						{
							swap(babies[j], babies[babies.size() - 1]);
							babies.pop_back();
							j--;
						}
					}
				}
				else {
					vector<vector<string> > babiesTemp(babies);
					for (int j = 0; j < babies.size(); j++)
					{
						/*
						if (checker.CheckDependency(fatherL[i], techtree, babies[j]) &&
							checker.CheckProducer(fatherL[i], techtree, babies[j]))
							babies[j].push_back(fatherL[i]);
						babies[j].push_back(fatherL[i]);
						if (pA.preCheck(babies[j])) {
						}
						else
						{
							swap(babies[j], babies[babies.size() - 1]);
							babies.pop_back();
							j--;
						}
					}

					for (int k = 0; k < babiesTemp.size(); k++) {
						/*
						if (checker.CheckDependency(motherL[i], techtree, babiesTemp[k]) &&
							checker.CheckProducer(motherL[i], techtree, babiesTemp[k]))
							babiesTemp[k].push_back(motherL[i]);
						babiesTemp[k].push_back(motherL[i]);
						if (pA.preCheck(babiesTemp[k])) {}
						else {
							swap(babiesTemp[k], babiesTemp[babiesTemp.size() - 1]);
							babiesTemp.pop_back();
							k--;
						}
					}
					babies.insert(babies.end(), babiesTemp.begin(), babiesTemp.end());
					int n_after = babies.size();
				}
			}
			else {
				vector<string> temp;
				if (fatherL[0] == motherL[0]) {
					temp.push_back(fatherL[0]);
					babies.push_back(temp);
				}
				else {
					temp.push_back(fatherL[0]);
					babies.push_back(temp);
					temp.pop_back();
					temp.push_back(motherL[0]);
					babies.push_back(temp);
				}
			}
			cout << "current length: " << i << endl;
		}*/

		vector<string> skeleton(length);
		int count = 0;
		for (int i = 0; i < length; i++) {
			if (fatherL[i] == motherL[i]) {
				skeleton[i] == fatherL[i];
				count++;
			}
		}
		
		int n = length - count;
		int MAXSIZE = pow(2, n);
		srand(time(0));
		while(babies.size()<min(MAXSIZE,100)){
			int i = rand() % MAXSIZE;
			bitset<sizeof(int)*8> index(i);
			vector<string> tempA(skeleton);
			vector<string> tempB(skeleton);
			int position = 0;
			for (int j = 0; j < length; j++) {
				if (tempA[j].empty()) {
					if (index[position]) {
						tempA[j] = fatherL[j];
						tempB[j] = motherL[j];
					}
					else
					{
						tempA[j] = motherL[j];
						tempB[j] = fatherL[j];
					}
					position++;
				}
			}
			if (pA.preCheck(tempA)) babies.push_back(tempA);
			if (pA.preCheck(tempB)) babies.push_back(tempB);

			//cout some information
			//if (babies.size() % 50 == 0) cout << "Made " << babies.size() << " babies" << endl;
		}

		return babies;
	}

private:
	string objectiveUnit;
	string ProductionStruct;
	string techTree = "protoss_techtree.csv";
	vector<tuple<string, string, string>> allUnits;
	int individual = 10000;
	int eliteNum;
	string baseCamp = "nexus";
	string worker = "probe";
	string supply = "pylon";
	string firstBarrack = "gateway";
	string assimilator = "assimilator";
	int vespene = 0;
	int refinery = 0;
};

#endif