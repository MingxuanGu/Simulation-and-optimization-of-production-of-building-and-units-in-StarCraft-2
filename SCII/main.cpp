#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "PreAction.h"
#include "Resources.h"
#include "Timer.h"
#include "JsonMaster.h"
#include "Buildings.h"
#include "BuildedList.h"
#include "Checker.h"
#include "Builder.h"
#include "ProbeManager.h"
using namespace std;

bool checkAndBuild(int i, Checker& checker, Builder& builder) {
	if (checker.CheckMaster(i))
	{
		//cout << "check complete" << endl;
		builder.build(i);
		return true;
	}
	else return false;
}


int main(int argc, char*argv[]) {
	string titles[] = { "name", "minerals", "vespene", "build_time", "supply_cost", "supply_provided", "start_energy", "max_energy", "race", "produced_by", "dependency" };

	PreAction pAction;
	vector<map<string, string>> toBuildList;
	string techTree;

	if (argc == 3)
	{
		//cout << "Usage:" << argv[0] << "SOURCE DESTINATION" << endl;
		//cout << "The argument is: " << argv[1] << " " << argv[2] << endl;
		string race = argv[1];
		//check race
		if (race == "sc2-hots-protoss") {
			techTree = "protoss_techtree.csv";
			vector<string> rawList = pAction.readList(argv[2]);
			Resources resources;
			Timer timer;
			JsonMaster jsonmaster(&timer, &resources);

			if (!pAction.preCheck(rawList, techTree)) {
				jsonmaster.invalid();
				jsonmaster.jsonPWriter();
				return 0;
			}
			else toBuildList = pAction.getDetailedBuildList();

			BuildedList buildedList;
			Checker checker(&timer, &resources, &buildedList, toBuildList);
			Builder builder(&timer, &resources, &buildedList, toBuildList);
			
			int listLength = toBuildList.size();
			ProbeManager probeManager(&resources, toBuildList, &buildedList, listLength);
			int maxTime = 1000;
			int i = 0;
			for (; timer.getCurrentTime() <= maxTime; timer.timeIncf())
			{
				resources.collectResources(buildedList.getProbeList());
				builder.proceed();//build one second
				probeManager.manageProbes(i);//adjust the number of probes in minerals and vespene
				if (!builder.chronoBoost()) {
					if (i < listLength) {
						if (checkAndBuild(i, checker, builder))
						{
							i++;
							//if (i == listLength) { cout << "the end of the build list" << endl; }
						}
					}
				}
				
				if (builder.hasEvents() || probeManager.probeChanged())
				{
					int workersInMineral = buildedList.workersInMineral();
					int workersInVespene = buildedList.workersInVespene();
					jsonmaster.statusMaster(workersInMineral, workersInVespene);
					if (builder.hasEvents())
					{
						vector<map<string, char*>>& events = builder.getEvents();
						jsonmaster.eventMaster(events);
						builder.cleanEvents();
					}
				}
			}
			if (!builder.proceedListEmpty() || i < listLength) {
				//cout << "Time exceed!" << endl;
				jsonmaster.invalid();
				builder.deleteProceedList();
				//delete &builder;
				//delete &checker;

			}
			jsonmaster.jsonPWriter();
		}
		else {
			//cout << "This is the simulation for sc2-hots-protoss! Plz input the corrent race name." << endl;
			return 0;
		}
	}
	else {
		//cout << "The argument input is incorrect." << endl;
		return 0;
	}
	return 0;
}

