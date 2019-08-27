#pragma once
#include <string>
#include <iostream>
#define CONSTRUCT "construct"
#define IDLE "idle"
using namespace std;
class building {
public:
	virtual void constructing() = 0;
	virtual void idle() = 0;
	virtual string getStatus() = 0;
	virtual string& getName() = 0;
	virtual float& getSpeed() = 0;
	virtual void energyRecovery() = 0;
	virtual void boost() = 0;
	virtual void boostEnd() = 0;
protected:
	string name;
	string status;
	float buildSpeed;
	float energy;
};
class nexus: public building {
public:
	nexus(string n) :status(IDLE), name(n), buildSpeed(1.0f), energy(0.0f), maxEnergy(100) {}
	void constructing() { status = CONSTRUCT; }
	void idle() { status = IDLE; }
	string getStatus() { return status; }
	string& getName() { return name; }
	float& getSpeed() { return buildSpeed; }
	void energyRecovery() { 
		if ((maxEnergy - energy) >= 0.5625)energy += 0.5625f;
		else energy = maxEnergy;
	}
	bool skillLaunchable() { if (energy >= 25)return true; else { return false; } }
	void launchCBoost() { energy -= 25; }
	void boost() { buildSpeed = 1.5f; }
	void boostEnd() { buildSpeed = 1.0f; }
private:
	string name;
	string status;
	float buildSpeed;
	float energy;
	float maxEnergy;
};
class gateway :public building {
public:
	gateway(string n) :status(IDLE), name(n), buildSpeed(1), energy(0.0f) {}
	void constructing() { status = CONSTRUCT; }
	void idle() { status = IDLE; }
	string getStatus() { return status; }
	string& getName() { return name; }
	float& getSpeed() { return buildSpeed; }
	void energyRecovery() { energy += 0.5625f; }
	void boost() { buildSpeed = 1.5f; }
	void boostEnd() { buildSpeed = 1.0f; }
private:
	string name;
	string status;
	float buildSpeed;
	float energy;
};
class robotics_facility : public building {
public:
	robotics_facility(string n) :status(IDLE), name(n), buildSpeed(1), energy(0.0f) {}
	void constructing() { status = CONSTRUCT; }
	void idle() { status = IDLE; }
	string getStatus() { return status; }
	string& getName() { return name; }
	float& getSpeed() { return buildSpeed; }
	void energyRecovery() { energy += 0.5625f; }
	void boost() { buildSpeed = 1.5f; }
	void boostEnd() { buildSpeed = 1.0f; }
private:
	string name;
	string status;
	float buildSpeed;
	float energy;
};
class stargate : public building {
public:
	stargate(string n) :status(IDLE), name(n), buildSpeed(1), energy(0.0f) {}
	void constructing() { status = CONSTRUCT; }
	void idle() { status = IDLE; }
	string getStatus() { return status; }
	string& getName() { return name; }
	float& getSpeed() { return buildSpeed; }
	void energyRecovery() { energy += 0.5625f; }
	void boost() { buildSpeed = 1.5f; }
	void boostEnd() { buildSpeed = 1.0f; }
private:
	string name;
	string status;
	float buildSpeed;
	float energy;
};