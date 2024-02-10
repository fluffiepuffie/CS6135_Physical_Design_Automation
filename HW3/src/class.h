#ifndef _CLASS_H_
#define _CLASS_H_
#include <bits/stdc++.h>
#include <vector>

using namespace std;

class Chip {
    public:
    int width = 0;
    int height = 0;
};

class Module {
    public:
    string type = "";
    string name = "";
    int x = 0;
    int y = 0;

    virtual int B() const;
    virtual int T() const;
    virtual int L() const;
    virtual int R() const;
    pair<int, int> getCenter() const;

};

class SoftModule:public Module {
    public:
    vector<pair<int, int>> candidateWH = {};
    int minSize = 0;
    bool placed = false;
    int WHInd = -1;
    int B() const override;
    int T() const override;
    int L() const override;
    int R() const override;
    bool compare(const SoftModule* self, const SoftModule* other) const;

    SoftModule(); 
    SoftModule(const SoftModule& other); 
};

class FixedModule:public Module {
    public:
    int height = 0;
    int width = 0;
    bool operator<(const FixedModule& other) const;

    int B() const override;
    int T() const override;
    int L() const override;
    int R() const override;
};

class Net{
    public:
    string module1Name;
    string module2Name;
    int netWeight = 0;
};



void readChip(Chip &);
void readSoftModule();
void readFixedModule();
void readNet(vector<Net*> &);
void initialSolution(unordered_map<string, SoftModule*> const &, unordered_map<string, FixedModule*> const &, vector<string> &);

#endif