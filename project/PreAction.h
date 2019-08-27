#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
using namespace std;

class PreAction {
public:
	PreAction():assimilator(0), needVespene(0){}
	//read the build list and store it in a vector<string>
	vector<string> readList(string buildList) {
		vector<string> toBuildList;
		ifstream outfile(buildList, ifstream::in);
		string store;
		while (!outfile.eof())
		{
			getline(outfile, store, '\n');
			if (store.length() < 2) continue;
			toBuildList.push_back(store);

		}
		outfile.close();
		return toBuildList;
	}
	//dependency precheck
	bool preCheck(vector<string> &toBuildList, string techTree) {
		string store;
		string success = "Congratulations! Passed the pre check!";
		string error = "Did not pass the pre check. The build list is invalid.";
		bool passCheck = true;
		string toBuild;
		int mothership = 0;
		for (int i = 0; i < toBuildList.size(); ++i)
		{
			string titles[] = {"name", "minerals", "vespene", "build_time", "supply_cost", "supply_provided", "start_energy", "max_energy", "race", "produced_by", "dependency"};
			toBuild = toBuildList[i];
			passCheck = true;
			ifstream outfile(techTree, ifstream::in);
			getline(outfile, store, '\n');
			getline(outfile, store, '\n');
			string type = "Unit";
			while (!outfile.eof())//if has not been the end of the file
			{
				
				getline(outfile, store, ',');
				//pass the "#building" line
				if (store == "#buildings")
				{
					type = "Building";
					getline(outfile, store, '\n');
					getline(outfile, store, ',');
				}
				//if match the name of the building or unit to be built
				if (store == toBuild)
				{
					//change the section to fit your own race
					if (store == "mothership") { 
						mothership++; 
						if (mothership > 1) {
							error.append("\nMore than one mothership.");
							passCheck = false; break;
						}
					}
					else if (store == "assimilator") {
						assimilator++;
						if (assimilator > 2) { 
							error.append("\nMore than two Assimilator.");
							passCheck = false; break; }
					}
					map<string, string> M;
					M.insert(pair<string, string>(titles[0], store));
					//store the whole line of the data
					for (int j = 1; j <= 9; ++j) {
						getline(outfile, store, ',');
						M.insert(pair<string, string>(titles[j], store));
						if (titles[j] == "vespene")
						{
							if (stoi(store) > 0)needVespene = 1;
						}
					}
					getline(outfile, store, ',');
					M.insert(pair<string, string>(titles[10], store));

					M.insert(pair<string, string>("type", type));
					detailedBuildList.push_back(M);
					//implement "produced_by" check
					bool producerCheck = false;
					for (int j = 0; j <= i; ++j) {
						if ((M["produced_by"] == toBuildList[j]) || (M["produced_by"] == "nexus")) { producerCheck = true; break; }
					}
					if (producerCheck == false){error.append("\nProducerCheck: Build a ").append(M["produced_by"]).append(" before building ").append(M["name"]);
}

					//implement dependency check
					if (store != "")
					{
						bool existCheck = false;
						for (int j = 0; j < i; ++j) {
							if (store == toBuildList[j]) { existCheck = true; break; }
						}
						if (existCheck == false){error.append("\nDependency check: Build a ").append(store).append(" before building ").append(M["name"]);
}
						passCheck = (existCheck&&producerCheck);
					}
					else { passCheck = producerCheck;  break; }
					getline(outfile, store, '\n');
				}
				//go to the end of the line
				else getline(outfile, store, '\n');
			}
			outfile.close();
			if (needVespene > 0) {
				if (assimilator == 0)
				{
					passCheck = false;
					error.append("\nSome units need vespene. No Assimilator in the list.");
				}
			}
			if (!passCheck)
			{
				//cout << error << endl;
				
				return false;
			}
		}
		//cout << success << endl;
		return true;
	}
	vector<map<string, string>> getDetailedBuildList() { return detailedBuildList; }
private:
	vector<map<string, string>> detailedBuildList;
	int assimilator;
	int needVespene;
};
