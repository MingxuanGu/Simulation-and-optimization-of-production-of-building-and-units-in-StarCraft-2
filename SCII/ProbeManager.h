#pragma once
#include "Resources.h"
#include "BuildedList.h"
#include "Probe.h"
#include <algorithm>
#include <vector>
#include <map>
using namespace std;
class ProbeManager {
public:
	ProbeManager(Resources* r, vector<map<string, string>>& tbl, BuildedList* bdl, int& lL) : resources(r), toBuildList(tbl), buildedList(bdl), mNeed(0), vNeed(0), listLength(lL), probeInM(6), probeInV(0), probeDisChanged(false), probeCounter(0) {}
	void manageProbes(int& tb) {
		tobuild = tb;
		int PIM = 0;
		int PIV = 0;
		if (tobuild < listLength) {
			if (buildedList->hasAssimilator())
			{
				probeInM = 0;
				probeInV = 0;
				for (Probe p : buildedList->getProbeList()) {
					if (p.getStatus() == VESPENE) probeInV++;
					else probeInM++;
				}
				int assimilator = buildedList->getAssimilator();
				int maxProbeInAssi = assimilator * 3;
				double minerals = resources->getMinerals();
				float vespene = resources->getVespene();
				mNeed += stof(toBuildList[tobuild]["minerals"]);
				vNeed += stof(toBuildList[tobuild]["vespene"]);
				if ((minerals >= mNeed) || (vespene >= vNeed))
				{
					if (tobuild < listLength - 1)
					{
						int next = tobuild + 1;
						manageProbes(next);
					}
					else if (vespene < vNeed)
					{
						probeCounter = 0;
						for (Probe& probe : buildedList->getProbeList()) {
							if (probeCounter < maxProbeInAssi) {
								probe.goVespene();
								probeCounter = probeCounter + 1;
							}
							else { probe.goMineral(); }
						}
						if (maxProbeInAssi != probeInV)
						{
							probeDisChanged = true;
						}
						else probeDisChanged = false;
					}
					else if (minerals < mNeed)
					{
						PIM = 0;
						for (Probe& probe : buildedList->getProbeList()) {
							probe.goMineral();
							PIM++;
						}
						if (PIM != probeInM) {
							probeDisChanged = true;
						}
						else probeDisChanged = false;
					}
				}
				else
				{
					int probeNumber = buildedList->getProbeList().size();
					double mTime = 0;
					double vTime = 0;
					int probeInVespene = 1;
					int time = 0;
					for (int i = 1; i <= maxProbeInAssi; i++)
					{
						vTime = (vNeed - vespene) / (i*0.35);
						mTime = (mNeed - minerals) / ((probeNumber - i)*0.7);
						int maxTime = max(vTime, mTime);
						if (time == 0) { time = maxTime; }
						else {
							if (time > maxTime) {
								time = maxTime; probeInVespene = i;
							}
						}
					}

					probeCounter = 0;
					for (Probe& probe : buildedList->getProbeList()) {
						if (probeCounter < probeInVespene)
						{
							probe.goVespene();
							probeCounter++;
						}
						else probe.goMineral();
					}
					if (probeInVespene != probeInV)
					{
						probeDisChanged = true;
					}
					else probeDisChanged = false;
				}
			}
			mNeed = 0;
			vNeed = 0;
		}
		else probeDisChanged = false;
	}
	bool probeChanged() { return probeDisChanged; }
private:
	vector<Probe> probes;
	Resources* resources;
	vector<map<string, string>> toBuildList;
	BuildedList* buildedList;
	int tobuild;
	double mNeed;
	float vNeed;
	int listLength;
	bool probeDisChanged;
	int probeInM;
	int probeInV;
	int probeCounter;
};