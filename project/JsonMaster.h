#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <math.h>
#include "Timer.h"
#include "Resources.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;
class JsonMaster {
public:
	JsonMaster(Timer* t, Resources* r):timer(t), resources(r) {
		d.Parse(protossjsonfile);
	}
	//push status into the json file
	//input parameters are the number of workers in mineral and vespene
	void statusMaster(int& workerInM, int& workerInV) {
		Value& messages = d["messages"];
		Document::AllocatorType& allocator = d.GetAllocator();

		Value message(kObjectType);
		Value status(kObjectType);
		Value workers(kObjectType);
		Value res(kObjectType);
		
		workers.AddMember("minerals", workerInM, allocator);
		workers.AddMember("vespene", workerInV, allocator);

		if ((ceil(resources->getMinerals()) - resources->getMinerals()) <= 0.01)
		{
			res.AddMember("minerals", (int)ceil(resources->getMinerals()), allocator);
		}
		else res.AddMember("minerals", (int)resources->getMinerals(), allocator);
		
		if ((ceil(resources->getVespene()) - resources->getVespene()) <= 0.01)
		{
			res.AddMember("vespene", (int)ceil(resources->getVespene()), allocator);
		}
		else res.AddMember("vespene", (int)resources->getVespene(), allocator);
		
		res.AddMember("supply-used", resources->getSupplyUsed(), allocator);
		res.AddMember("supply", resources->getSupply(), allocator);
		status.AddMember("workers", workers, allocator);
		status.AddMember("resources", res, allocator);

		message.AddMember("time", (int)(timer->getCurrentTime()), allocator);
		message.AddMember("status", status, allocator);
		messages.PushBack(message, allocator);
	}

	/*template of the event map:
	{
		"type": "build_start"
		"name": "scv"
		"producerID": "init_command_center"
	}*/
	/*{
		"type": "build-end",
		"name" : "probe",
		"producerID" : "init_nexus",
		"producedIDs" : "probe1,probe2"
		]
	}*/
	/*{
		"type": "special",
		"name" : "chronoboost",
		"triggeredBy" : "init_nexus",
		"targetBuilding" : "gateway"
	}*/
	//push events to the json file
	void eventMaster(std::vector<std::map<string, char*>>vEvents) {
		Value& messages = d["messages"];
		Document::AllocatorType& allocator = d.GetAllocator();

		int last = messages.Size();
		Value& message = messages[last - 1];
		Value events(kArrayType);
		for (std::map<string, char*> mEvent : vEvents) {
			char* type = mEvent["type"];
			Value event(kObjectType);
			Value str;
			str = StringRef(type);
			event.AddMember("type", str, allocator);
			str = StringRef(mEvent["name"]);

			event.AddMember("name", str, allocator);
			//when type id "build-start"
			if (type == "build-start")
			{
				str = StringRef(mEvent["producerID"]);
				event.AddMember("producerID", str, allocator);
			}
			//when type id "build-end"
			else if (type == "build-end") {
				char deli = ',';
				std::string sproducedIDs = "";
				sproducedIDs.append(mEvent["producedIDs"]);
				event.AddMember("producerID", StringRef(mEvent["producerID"]), allocator);
				Value producedIDs(kArrayType);
				size_t found = sproducedIDs.find(deli);
				if (found != std::string::npos) {
					while (found != std::string::npos) {
						std::string sub = sproducedIDs.substr(0, found);
						char* cstr = new char[sub.length()];
						strcpy(cstr, sub.c_str());
						producedIDs.PushBack(StringRef(cstr), allocator);
						sproducedIDs.erase(0, found + 1);
						found = sproducedIDs.find(deli);
					}
					char* cstr = new char[sproducedIDs.length()];
					strcpy(cstr, sproducedIDs.c_str());
					producedIDs.PushBack(StringRef(cstr), allocator);
				}
				else { 
					producedIDs.PushBack(StringRef(mEvent["producedIDs"]), allocator);
				}
				event.AddMember("producedIDs", producedIDs, allocator);
			}
			//when type is "special"
			else if (type == "special") {
				event.AddMember("triggeredBy", StringRef(mEvent["triggeredBy"]), allocator);
				event.AddMember("targetBuilding", StringRef(mEvent["targetBuilding"]), allocator);
			}
			events.PushBack(event, allocator);
		}
		message.AddMember("events", events, allocator);
	}

	//precheck invalid
	void invalid() {
		//Value& buildlistValid = d["buildlistValid"];
		d.Parse(failJson);
		//buildlistValid.SetInt(0);
	}
	//print the json on stdout in a pretty form
	void jsonPWriter() {
		StringBuffer buffer(0, 65536);
		PrettyWriter<StringBuffer> writer(buffer);
		d.Accept(writer);
		// std Output
		std::cout << buffer.GetString() << std::endl;
	}
private:
	Timer* timer;
	Resources* resources;
	//change the jsonfile to fit your own race
	const char* protossjsonfile = "{\
		\"buildlistValid\": 1,\
		\"game\": \"sc2-hots-protoss\",\
		\"initialUnits\": {\
			\"nexus\": [\"init_nexus\"],\
			\"probe\": [\
				\"init_probe_01\",\
				\"init_probe_02\",\
				\"init_probe_03\",\
				\"init_probe_04\",\
				\"init_probe_05\",\
				\"init_probe_06\"\
			]\
		},\
		\"messages\": []\
	}";
	const char* failJson = "{\
		\"buildlistValid\": 0,\
		\"game\": \"sc2-hots-protoss\"\
	}";
	Document d;
	
};