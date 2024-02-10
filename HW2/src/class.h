#include<bits/stdc++.h>
#include <boost/algorithm/string.hpp>
//dsajfd

using namespace std;
//dsjfla

class LibCell {
public:
    string name;
    unsigned long size;

    void config(string n, int s) {
        name = n;
        size = s;
    }
};

class Tech {
public:
    string name;
    unordered_map<string, LibCell> libCells = {};

    void config(string n){
        name = n;
    }

    void addLibCell(LibCell libCell) {
        libCells[libCell.name] = libCell;
    }
};

class Cell {
public:
    string name;
    string libCellName;
    long gain = 0;
    string assignedDie;  
    int locked = 0;
    list<string>::iterator bucketListPointer;
    unordered_set<string> connectedNets = {}; // cell array 
    void config(string n, string lbn) {
        name = n;
        libCellName = lbn;
    }
    void addConnectedNet(string Net) {
        connectedNets.insert(Net);
    }
    
};

class Die {
public:
    string name;
    string techName;
    unsigned long long maxSize;
    unsigned long long usedSize = 0;
    int maxUtilization;  
    unordered_set<string> assignedCells = {};
    vector<list<string>> bucketList = {};
    long maxGainIndex = 0;
    void config(string n, string tn, long a, int maxUtil) {
        name = n;
        techName = tn;
        maxSize = a;
        maxUtilization = maxUtil;
        usedSize = 0;
        maxGainIndex = 0;
    }
};



class Net {
public:
    string name;
    int numConnections;
    unordered_set<string> connectedCells = {}; //net array
    unordered_set<string> connectedCellsOnDieA = {};  
    unordered_set<string> connectedCellsOnDieB = {};  
    unordered_set<string> freeCells = {};
    unordered_set<string> lockedCells = {};

    void config(string n, int num){
        name = n;
        numConnections = num;
    }

    void addConnectedCell(string cellName) {
        connectedCells.insert(cellName);
    }
};



void readTech(unordered_map<string, Tech*> &);
void testReadTech(unordered_map<string, Tech*> &);
void readDie(unordered_map<string, Die*> &);
void testReadDie(unordered_map<string, Die*> &);
void readCell(unordered_map<string, Cell*> &);
void testReadCell(unordered_map<string, Cell*> &);
void readNets(unordered_map<string, Cell*> &, unordered_map<string, Net> &);
void testReadNet(unordered_map<string, Cell*> &, unordered_map<string, Net> &);

void initialSolution(unordered_map<string, Die*> &, unordered_map<string, Tech*> &, unordered_map<string, Cell*> &, unordered_map<string, Net> &);
void testSolution(unordered_map<string, Tech*> &,unordered_map<string, Die*> &, unordered_map<string, Cell*> &, unordered_map<string, Net> &); 
void reconfigCellAndNet(unordered_map<string, Die*> &, unordered_map<string, Cell*> &, unordered_map<string, Net> &); 
void computeInitialGain(unordered_map<string, Die*> &, unordered_map<string, Cell*> &, unordered_map<string, Net> &);
void testInitialGain(unordered_map<string, Cell*> &);
void testBucketList(unordered_map<string, Die*> &); 
void moveAndUpdate(unordered_map<string, Tech*> &, unordered_map<string, Die*> &, unordered_map<string, Cell*> &, unordered_map<string, Net> &, vector<long> &, vector<unordered_map<string, Die*>> &);

