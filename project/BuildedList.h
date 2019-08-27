#pragma once
#include <vector>
#include "Probe.h"
#include "Buildings.h"
#define MINERALS "minerals"
#define VESPENE "vespene"
#define CONSTRUCT "construct"
#define IDLE "idle"
class BuildedList {
public:
	BuildedList() :probe(0), zealot(0), stalker(0), sentry(0), warp_prism(0), immortal(0), observer(0), colossus(0), high_templar(0), dark_templar(0), mothership(0), phoenix(0), void_ray(0), carrier(0), pylon(0), pylonName(0), gway(0), cybernetics_core(0), cybernetics_coreName(0), robotics_bay(0), robotics_bayName(0), twilight_council(0), twilight_councilName(0), templar_archives(0), templar_archivesName(0), dark_shrine(0), dark_shrineName(0), fleet_beacon(0), fleet_beaconName(0), assimilator(0), forge(0), forgeName(0), photon_cannon(0), assimNumber(0), sgate(0), robotfac(0){
		for (int i = 1; i <= 6; ++i) {
			Probe probe0("init_probe_0" + '0' + i, MINERALS);
			probeList.push_back(probe0);
			probe++;
		}
		nexus nex("init_nexus");
		nexusList.push_back(nex);
	}
	int workersInMineral() {
		int i = 0;
		for(Probe probe: probeList)
		{
			if (probe.getStatus() == MINERALS) ++i;
		}
		return i;
	}
	int workersInVespene() {
		int i = 0;
		for (Probe probe : probeList)
		{
			if (probe.getStatus() == VESPENE) ++i;
		}
		return i;
	}

	vector<Probe>& getProbeList() { return probeList; }
	int buildProbe() { probe++; return probe; }
	int buildZealot() { zealot++; return zealot; }
	int buildStalker() { stalker++; return stalker; }
	int buildSentry() { sentry++; return sentry; }
	int buildWarp_prism() { warp_prism++; return warp_prism; }
	int buildImmortal() { immortal++; return immortal; }
	int buildObserver() { observer++; return observer; }
	int buildColossus() { colossus++; return colossus; }
	int buildHigh_templar() { high_templar++; return high_templar; }
	int builDark_templar() { dark_templar++; return dark_templar; }
	int buildMothership() { mothership++; return mothership; }
	int buildPhoenix() { phoenix++; return phoenix; }
	int buildVoid_ray() { void_ray++; return void_ray; }
	int buildCarrier() { carrier++; return carrier; }

	int buildNexus() { nex++; return nex; }
	int buildPylon() { pylonName++; return pylonName; }
	int buildGateway() { gway++; return gway; }
	int buildCyber_core() { cybernetics_coreName++; return cybernetics_coreName; }
	int buildRobFac() { robotfac++; return robotfac; }
	int buildRobBay() { robotics_bayName++; return robotics_bayName; }
	int buildTwiCon() { twilight_councilName++; return twilight_councilName; }
	int buildTemArch() { templar_archivesName++; return templar_archivesName; }
	int buildDarkShrine() { dark_shrineName++; return dark_shrineName; }
	int buildStargate() { sgate++; return sgate; }
	int buildFleetBeacon() { fleet_beaconName++; return fleet_beaconName; }
	int buildAssimilator() { assimilator++; return assimilator; }
	int buildForge() { forgeName++; return forgeName; }
	int buildPhotonCannon() { photon_cannon++; return photon_cannon; }

	void addPylon() { pylon++; }
	void addCyberCore() { cybernetics_core++; }
	void addRobBay() { robotics_bay++; }
	void addTwiCon() { twilight_council++; }
	void addTempArch() { templar_archives++; }
	void addDarkShrine() { dark_shrine++; }
	void addFleetBeacon() { fleet_beacon++; }
	void addAssimilator() { assimNumber++; }
	void addForge() { forge++; }

	vector<nexus>& getNexusList() { return nexusList; }
	bool hasPylon() { if (pylon > 0)return true; else return false; }
	vector<gateway>& getGatewayList() { return gatewayList; };
	bool hasCyber_core() { if (cybernetics_core > 0)return true; else return false; }
	vector<robotics_facility>& getRobfacList() { return robfacList; };
	bool hasRobBay() { if (robotics_bay > 0)return true; else return false; }
	bool hasTwiCon() { if (twilight_council > 0)return true; else return false; }
	bool hasTemArch() { if (templar_archives > 0)return true; else return false; }
	bool hasDarkShrine() { if (dark_shrine > 0)return true; else return false; }
	vector<stargate>& getStargateList() { return stargateList; };
	bool hasFleetBeacon() { if (fleet_beacon > 0)return true; else return false; }
	bool hasAssimilator() { if (assimNumber > 0)return true; else return false; }
	int getAssimilator() { return assimNumber; }
	bool hasForge() { if (forge > 0)return true; else return false; }
private:
	vector<Probe> probeList;
	int probe;
	int zealot;
	int stalker;
	int sentry;
	int warp_prism;
	int immortal;
	int observer;
	int colossus;
	int high_templar;
	int dark_templar;
	int mothership;
	int phoenix;
	int void_ray;
	int carrier;

	int nex;
	vector<nexus> nexusList;
	int pylon;
	int pylonName;
	int gway;
	vector<gateway> gatewayList;
	int cybernetics_core;
	int cybernetics_coreName;
	int robotfac;
	vector<robotics_facility> robfacList;
	int robotics_bay;
	int robotics_bayName;
	int twilight_council;
	int twilight_councilName;
	int templar_archives;
	int templar_archivesName;
	int dark_shrine;
	int dark_shrineName;
	int sgate;
	vector<stargate> stargateList;
	int fleet_beacon;
	int fleet_beaconName;
	int assimilator;
	int assimNumber;
	int forge;
	int forgeName;
	int photon_cannon;
};