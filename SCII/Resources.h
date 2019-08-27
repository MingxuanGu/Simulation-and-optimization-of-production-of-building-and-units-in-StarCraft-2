#pragma once
#include "Probe.h"
#include <string>
using namespace std;

class Resources {
public:
	Resources():minerals(50), vespene(0), supply_used(6), supply(10) {}
	void addMinerals(float amount) { minerals += amount; }
	void addVespene(float amount) { vespene += amount; }
	void addSuppluUsed(int amount) { supply_used += amount; }
	void addSupply(int amount) { supply += amount; }
	void subMinerals(float amount) { minerals -= amount; }
	void subVespene(float amount) { vespene -= amount; }
	void subSuppluUsed(int amount) { supply_used -= amount; }
	void subSupply(int amount) { supply -= amount; }
	double getMinerals() { return minerals; }
	float getVespene() { return vespene; }
	int getSupplyUsed() { return supply_used; }
	int getSupply() { return supply; }
	void collectResources(vector<Probe>& probeList) {
		for (Probe probe : probeList) {
			string status = probe.getStatus();
			if (status == MINERALS) minerals += 0.7;
			else if (status == VESPENE) vespene += 0.35;
		}
	}
private:
	double minerals;
	float vespene;
	int supply_used;
	int supply;
};