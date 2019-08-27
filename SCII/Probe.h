#pragma once
#include <string>
#define MINERALS "minerals"
#define VESPENE "vespene"
#define CONSTRUCT "construct"
#define IDLE "idle"
using namespace std;

class Probe {
public:
	Probe(string n, string s):name(n), status(s) {}
	string getName() { return name; }
	string getStatus() { return status; }
	void goMineral() { status = MINERALS; }
	void goVespene() { status = VESPENE; }
private:
	string name;
	string status;
	int wMinerals;
	int wVespene;
};