#pragma once
#include "Timer.h"
#include "Resources.h"
#include "BuildedList.h"
#include <vector>
#include <map>
#include <thread>
#include <iostream>
#include "Buildings.h"
#define IDLE "idle"
#define MINERALS "minerals"
#define VESPENE "vespene"
#define CONSTRUCT "construct"
using namespace std;
class Checker {
public:
	Checker(Timer* t, Resources* r, BuildedList* b, vector<map<string, string>> tobulist) : timer(t), resources(r), buildedList(b), toBuildList(tobulist) {}
	bool CheckMaster(int i) {
		map<string, string> item = toBuildList[i];
		string name = item["name"];
		int minerals = stoi(item["minerals"]);
		int vespene = stoi(item["vespene"]);
		int supply_cost = stoi(item["supply_cost"]);
		string produced_by = item["produced_by"];
		string dependency = item["dependency"];
		string type = item["type"];
		if (resourceSupplyCheck(minerals, vespene, supply_cost)) {
			if (dependencyCheck(dependency)) {
				if (type == "Unit") return availCheck(produced_by);
				else return true;
			}
		}
		return false;
	}
	bool resourceSupplyCheck(int minerals, int vespene, int supply_cost) {
		if ((resources->getMinerals() >= minerals) && (resources->getVespene() >= vespene) && ((resources->getSupply() - resources->getSupplyUsed()) >= supply_cost)) return true;
		else return false;
	}
	bool dependencyCheck(string d) {
		if (d == "") return true;
		else if (d == "pylon")	return buildedList->hasPylon();
		else if (d == "gateway") return (!buildedList->getGatewayList().empty());
		else if (d == "cybernetics_core") return buildedList->hasCyber_core();
		else if (d == "robotics_facility")return (!buildedList->getRobfacList().empty());
		else if (d == "robotics_bay") return buildedList->hasRobBay();
		else if (d == "twilight_council") return buildedList->hasTwiCon();
		else if (d == "templar_archives") return buildedList->hasTemArch();
		else if (d == "dark_shrine")return buildedList->hasDarkShrine();
		else if (d == "stargate") return (!buildedList->getStargateList().empty());
		else if (d == "fleet_beacon") return buildedList->hasFleetBeacon();
		else if (d == "forge") return buildedList->hasForge();
		else return false;
	}
	bool availCheck(string pb) {
		if (pb == "nexus") {
			vector<nexus>& n = buildedList->getNexusList();
			for (nexus nex : n)
			{
				if (nex.getStatus() == IDLE) return true;
			}
			return false;
		}
		else if (pb == "gateway") {
			vector<gateway>& gate = buildedList->getGatewayList();
			for (gateway g : gate) {
				if (g.getStatus() == IDLE) return true;
			}
			return false;
		}
		else if (pb == "robotics_facility") {
			vector<robotics_facility>& rob = buildedList->getRobfacList();
			for (robotics_facility r : rob) {
				if (r.getStatus() == IDLE) return true;
			}
			return false;
		}
		else if (pb == "stargate") {
			vector<stargate>& star = buildedList->getStargateList();
			for (stargate s : star) {
				if (s.getStatus() == IDLE) return true;
			}
			return false;
		}
		else return false;
	}
	vector<map<string, string>> getEvents() { return events; }
private:
	Timer* timer;
	Resources* resources;
	BuildedList* buildedList;
	vector<map<string, string>> toBuildList;
	vector<map<string, string>> events;
};
