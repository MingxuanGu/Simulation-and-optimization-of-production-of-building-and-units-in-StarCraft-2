#pragma once
#include "BuildedList.h"
#include "Timer.h"
#include "Resources.h"
#include "Buildings.h"
#include <map>
#include <vector>
#include <thread>
#include <functional>
#include <algorithm> 

using namespace std;



class proceedManager {
public:
	proceedManager(building* p, string& pdID, string& pN, float& bT, BuildedList* bdl, int& sp) : producedID(pdID), producedName(pN), buildTime(bT), startTime(0), buildedList(bdl), supplyProvided(sp), probe("probe1") {
		if (p != NULL)producer = p;
		else producer = NULL;
	}
	bool proceedOneSec() {
		if (producer != NULL)
		{
			startTime = startTime + producer->getSpeed();
		}
		else {
			startTime++;
		}
		if (startTime >= buildTime)
		{
			if (producer != NULL)
			{
				producer->idle();
				if (producedName == "probe")
				{
					buildedList->getProbeList().push_back(Probe(producedID, MINERALS));
				}
			}
			else
			{
				if (producedName == "nexus")
				{
					buildedList->getNexusList().push_back(nexus(producedID));
				}
				else if (producedName == "pylon")
				{
					buildedList->addPylon();
				}
				else if (producedName == "gateway")
				{
					buildedList->getGatewayList().push_back(gateway(producedID));
				}
				else if (producedName == "cybernetics_core")
				{
					buildedList->addCyberCore();
				}
				else if (producedName == "robotics_facility")
				{
					buildedList->getRobfacList().push_back(robotics_facility(producedID));
				}
				else if (producedName == "robotics_bay")
				{
					buildedList->addRobBay();
				}
				else if (producedName == "twilight_council")
				{
					buildedList->addTwiCon();
				}
				else if (producedName == "templar_archives")
				{
					buildedList->addTempArch();
				}
				else if (producedName == "dark_shrine")
				{
					buildedList->addDarkShrine();
				}
				else if (producedName == "stargate")
				{
					buildedList->getStargateList().push_back(stargate(producedID));
				}
				else if (producedName == "fleet_beacon")
				{
					buildedList->addFleetBeacon();
				}
				else if (producedName == "assimilator") {
					buildedList->addAssimilator();
				}
				else if (producedName == "forge")
				{
					buildedList->addForge();
				}
			}
			return true;
		}
		else return false;
	}
	string& getProducedID() { return producedID; }
	string& getProducedName() { return producedName; }
	string& getProducerID() {
		if (producer == NULL)
		{
			return probe;
		}
		else return producer->getName();
	}
	int& getSupplyProvided() { return supplyProvided; }
	float getRemainTime() { return (buildTime - startTime); }
	building* getProducer() { return producer; }
private:
	building* producer;
	string producedID;
	string producedName;
	float buildTime;
	float startTime;
	BuildedList* buildedList;
	int supplyProvided;
	string probe;
};
class ChronoBoost {
public:
	ChronoBoost(building* b) : _building(b), timeRemain(20), timeUp(false) {}
	bool passOneSec() {
		timeRemain--;
		if (timeRemain == 1) timeUp = true;
	}
	void boostEnd() { _building->boostEnd(); }
	bool checktimeUp() { return timeUp; }
private:
	building* _building;
	int timeRemain;
	bool timeUp;
};

class Builder {
public:
	Builder(Timer* t, Resources* r, BuildedList* b, vector<map<string, string>> tbl) :timer(t), resources(r), toBuildList(tbl), buildedList(b) {}
	void build(int i) {
		map<string, string> item = toBuildList[i];
		string& name = item["name"];
		string producedBy = item["produced_by"];
		string producedID = "";
		building* IDLEBuilding;
		float build_time = stof(item["build_time"]);
		int supplyProvided = stoi(item["supply_provided"]);
		if (producedBy == "probe")
		{
			if (name == "nexus")
			{
				producedID = "nexus";
				producedID += ('0' + buildedList->buildNexus());
			}
			else if (name == "gateway")
			{
				producedID = "gateway";
				producedID += ('0' + buildedList->buildGateway());
			}
			else if (name == "robotics_facility")
			{
				producedID = "robotics_facility";
				producedID += ('0' + buildedList->buildRobFac());
			}
			else if (name == "stargate")
			{
				producedID = "stargate";
				producedID += ('0' + buildedList->buildStargate());
			}
			else if (name == "pylon")
			{
				producedID = "pylon";
				producedID += ('0' + buildedList->buildPylon());
			}
			else if (name == "cybernetics_core")
			{
				producedID = "cybernetics_core";
				producedID += ('0' + buildedList->buildCyber_core());
			}
			else if (name == "robotics_bay")
			{
				producedID = "robotics_bay";
				producedID += ('0' + buildedList->buildRobBay());
			}
			else if (name == "twilight_council")
			{
				producedID = "twilight_council";
				producedID += ('0' + buildedList->buildTwiCon());
			}
			else if (name == "templar_archives")
			{
				producedID = "templar_archives";
				producedID += ('0' + buildedList->buildTemArch());
			}
			else if (name == "dark_shrine")
			{
				producedID = "dark_shrine";
				producedID += ('0' + buildedList->buildDarkShrine());
			}
			else if (name == "fleet_beacon")
			{
				producedID = "fleet_beacon";
				producedID += ('0' + buildedList->buildFleetBeacon());
			}
			else if (name == "assimilator")
			{
				producedID = "assimilator";
				producedID += ('0' + buildedList->buildAssimilator());
			}
			else if (name == "forge")
			{
				producedID = "forge";
				producedID += ('0' + buildedList->buildForge());
			}
			else if (name == "photon_cannon")
			{
				producedID = "photon_cannon";
				producedID += ('0' + buildedList->buildPhotonCannon());
			}
			IDLEBuilding = NULL;
			proceedManager pM(IDLEBuilding, producedID, name, build_time, buildedList, supplyProvided);
			proceedList.push_back(pM);
		}
		else if (producedBy == "nexus")
		{
			if (name == "probe") {
				producedID = "probe";
				producedID += ('0' + buildedList->buildProbe());
			}
			else if (name == "mothership")
			{
				producedID = "mothership";
				producedID += ('0' + buildedList->buildMothership());
			}
			for (nexus& n : buildedList->getNexusList())
			{
				if (n.getStatus() == IDLE)
				{
					IDLEBuilding = &n;
					IDLEBuilding->constructing();
					proceedManager pM(IDLEBuilding, producedID, name, build_time, buildedList, supplyProvided);
					proceedList.push_back(pM);
					break;
				}
			}

		}
		else if (producedBy == "gateway")
		{
			if (name == "zealot")
			{
				producedID = "zealot";
				producedID += ('0' + buildedList->buildZealot());

			}
			else if (name == "stalker")
			{
				producedID = "stalker";
				producedID += ('0' + buildedList->buildStalker());
			}
			else if (name == "sentry")
			{
				producedID = "sentry";
				producedID += ('0' + buildedList->buildSentry());
			}
			else if (name == "high_templar")
			{
				producedID = "high_templar";
				producedID += ('0' + buildedList->buildHigh_templar());
			}
			else if (name == "dark_templar")
			{
				producedID = "dark_templar";
				producedID += ('0' + buildedList->builDark_templar());
			}
			for (gateway& g : buildedList->getGatewayList())
			{
				if (g.getStatus() == IDLE)
				{
					IDLEBuilding = &g;
					IDLEBuilding->constructing();
					proceedManager pM(IDLEBuilding, producedID, name, build_time, buildedList, supplyProvided);
					proceedList.push_back(pM);
					//n.constructing();
					//buildingID = IDLEBuilding->getName();
					break;
				}
			}
		}
		else if (producedBy == "robotics_facility")
		{
			if (name == "warp_prism")
			{
				producedID = "warp_prism";
				producedID += ('0' + buildedList->buildWarp_prism());

			}
			else if (name == "immortal")
			{
				producedID = "immortal";
				producedID += ('0' + buildedList->buildImmortal());
			}
			else if (name == "observer")
			{
				producedID = "observer";
				producedID += ('0' + buildedList->buildObserver());
			}
			else if (name == "colossus")
			{
				producedID = "colossus";
				producedID += ('0' + buildedList->buildColossus());
			}
			for (robotics_facility& r : buildedList->getRobfacList())
			{
				if (r.getStatus() == IDLE)
				{
					IDLEBuilding = &r;
					IDLEBuilding->constructing();
					proceedManager pM(IDLEBuilding, producedID, name, build_time, buildedList, supplyProvided);
					proceedList.push_back(pM);
					//n.constructing();
					//buildingID = IDLEBuilding->getName();
					break;
				}
			}
		}
		else if (producedBy == "stargate")
		{
			if (name == "phoenix")
			{
				producedID = "phoenix";
				producedID += ('0' + buildedList->buildPhoenix());

			}
			else if (name == "void_ray")
			{
				producedID = "void_ray";
				producedID += ('0' + buildedList->buildVoid_ray());
			}
			else if (name == "carrier")
			{
				producedID = "carrier";
				producedID += ('0' + buildedList->buildCarrier());
			}
			for (stargate& s : buildedList->getStargateList())
			{
				if (s.getStatus() == IDLE)
				{
					IDLEBuilding = &s;
					IDLEBuilding->constructing();
					proceedManager pM(IDLEBuilding, producedID, name, build_time, buildedList, supplyProvided);
					proceedList.push_back(pM);
					break;
				}
			}
		}
		if (IDLEBuilding == NULL) { string probe = "init_probe_01"; buildStartEvent(name, probe); }
		else {
			string& buildingID = IDLEBuilding->getName();
			//form build-start event
			buildStartEvent(name, buildingID);
		}

		int minerals = stoi(item["minerals"]);
		int vespene = stoi(item["vespene"]);
		int supply_cost = stoi(item["supply_cost"]);
		resources->subMinerals(minerals);
		resources->subVespene(vespene);
		resources->addSuppluUsed(supply_cost);
	}



	bool hasEvents() {
		if (events.size() > 0) return true;
		else return false;
	}
	void cleanEvents() { vector<map<string, char*>>().swap(events); }

	void buildStartEvent(string& name, string& producerID) {
		map<string, char*> event_;
		event_.insert(pair<string, char*>("type", "build-start"));
		c = new char[20];
		strcpy(c, name.c_str());
		event_.insert(pair<string, char*>("name", c));

		c = new char[20];
		strcpy(c, producerID.c_str());
		event_.insert(pair<string, char*>("producerID", c));

		events.push_back(event_);
	}
	void buildEndEvent(string& name, string& producerID, string& producedIDs) {
		map<string, char*> event;
		event.insert(pair<string, char*>("type", "build-end"));

		c = new char[20];
		strcpy(c, name.c_str());
		event.insert(pair<string, char*>("name", c));

		c = new char[20];
		strcpy(c, producerID.c_str());
		event.insert(pair<string, char*>("producerID", c));

		c = new char[20];
		strcpy(c, producedIDs.c_str());
		event.insert(pair<string, char*>("producedIDs", c));
		events.push_back(event);
	}
	void specialEvent(string& triggeredBy, string& targetBuilding) {
		map<string, char*> event_;
		event_.insert(pair<string, char*>("type", "special"));
		event_.insert(pair<string, char*>("name", "chronoboost"));

		c = new char[20];
		strcpy(c, triggeredBy.c_str());
		event_.insert(pair<string, char*>("triggeredBy", c));
		c = new char[20];
		strcpy(c, targetBuilding.c_str());
		event_.insert(pair<string, char*>("targetBuilding", c));
		events.push_back(event_);
	}
	bool chronoBoost() {
		cleanBoostList();
		for (ChronoBoost& cB : boostList)cB.passOneSec();
		building* toboost;
		bool hasSuchBuilding = false;
		for (nexus& n : buildedList->getNexusList()) {
			n.energyRecovery();
			if (n.skillLaunchable())
			{
				int remainTime = 0;
				for (proceedManager pM : proceedList) {
					if ((pM.getProducer() != NULL) && (pM.getRemainTime() > remainTime)) {
						if (pM.getProducer()->getSpeed() != 1.5)
						{
							hasSuchBuilding = true;
							remainTime = pM.getRemainTime();
							toboost = pM.getProducer();
						}
					}
				}
				int lowerBound = min(16, (int)(1000 - timer->getCurrentTime()));
				if ((remainTime >= lowerBound) && hasSuchBuilding)
				{
					n.launchCBoost();
					toboost->boost();
					specialEvent(n.getName(), toboost->getName());
					boostList.push_back(ChronoBoost(toboost));
					return true;
				}
				else return false;
			}
			else return false;
		}
	}
	void cleanBoostList() {
		for (auto i = boostList.begin(); i != boostList.end(); ++i) {
			if ((*i).checktimeUp())
			{
				(*i).boostEnd();
				boostList.erase(i);
				cleanBoostList();
				break;
			}
		}
	}

	vector<map<string, char*>>& getEvents() { return events; }
	void proceed() {
		vector<string> producedIDs;
		if (!proceedList.empty()) {
			for (proceedManager& pM : proceedList) {
				if (pM.proceedOneSec())
				{
					resources->addSupply(pM.getSupplyProvided());
					buildEndEvent(pM.getProducedName(), pM.getProducerID(), pM.getProducedID());
					producedIDs.push_back(pM.getProducedID());
				}
			}
		}
		if (!producedIDs.empty())
		{
			for (string& ID : producedIDs) {
				for (vector<proceedManager>::iterator iter = proceedList.begin(); iter != proceedList.end(); ++iter) {
					if ((*iter).getProducedID() == ID)
					{
						proceedList.erase(iter);
						break;
					}
				}
			}
		}
	}
	bool proceedListEmpty() {
		return proceedList.empty();
	}
	vector<proceedManager>& getproceedList() {
		return proceedList
			;
	}
	void deleteProceedList() {
		delete &proceedList;
	}
private:
	BuildedList* buildedList;
	Timer* timer;
	Resources* resources;
	vector<map<string, string>> toBuildList;
	vector<map<string, char*>> events;
	building* buildingPtr;
	vector<proceedManager> proceedList;
	char* c;
	vector<ChronoBoost> boostList;
};