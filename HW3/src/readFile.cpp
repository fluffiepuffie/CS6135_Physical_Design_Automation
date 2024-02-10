#include "class.h"

extern ifstream input;
extern vector<FixedModule *> fixedModules;
extern vector<SoftModule *> softModules;
extern unordered_map<string, FixedModule*> fixedModulesMap;
extern unordered_map<string, int> softModulesMap;
extern vector<Module> modules;

bool compareSoft(const SoftModule* self, const SoftModule* other) {
    return (self->R() - self->L()) * (self->T() - self->B()) > (other->R() - other->L()) * (other->T() - other->B());
}

bool compareFixed(const FixedModule *self, const FixedModule *other) {
    return self->y < other->y;
}

void readChip(Chip &chip) {
    string line;
    string null;
    // cout << line.str();
    getline(input, line);
    stringstream s(line);
    s >> null;
    s >> chip.width;
    s >> chip.height;
}

void readSoftModule() {
    string line;
    string null;
    int softModNum;
    getline(input, line); //read blank line
    getline(input, line);
    stringstream s(line);
    s >> null;
    s >> softModNum;
    for(int i = 0; i < softModNum; i++) {
        getline(input, line);
        stringstream s(line);
        SoftModule *sm = new SoftModule;
        s >> null;
        s >> sm->name;
        s >> sm->minSize;
        do{
            for(double i = sqrt(sm->minSize * 0.5); i < sqrt(sm->minSize * 2.0) + 1; i += 1) {
                // cout << i << endl;
                if(sm->minSize % int(i) != 0) {
                    continue;
                }
                int h = i;
                int w = sm->minSize/h;
                if(double(h)/double(w) >= 0.5 && double(h)/double(w) <= 2.0) {
                    // cout << h << " " << w << endl;
                    // sm.candidateH.push_back(h);
                    // sm.candidateW.push_back(w);
                    sm->candidateWH.push_back({w, h});
                }
            }
            if(sm->candidateWH.size() == 0)
                sm->minSize += 10;
        } while(sm->candidateWH.size() == 0);
        sm->WHInd = 0;
        sm->type = "soft";
        // sm->width = sm->candidateWH[0].first;
        // sm->height = sm->candidateWH[0].second;
        softModules.push_back(sm);
        // cout << sm.name << " " << sm->minSize << " " << sm->type << " " << sm->candidateH.size() << " " << sm->candidateW.size() << endl;
    }
    sort(softModules.begin(), softModules.end(), compareSoft);
    for(int i = 0; i < softModules.size(); i++) {
        softModulesMap[softModules[i]->name] = i;
    }
}

void readFixedModule() {
    string line;
    string null;
    int fixedModNum;
    getline(input, line); //read blank line
    getline(input, line);
    stringstream s(line);
    s >> null;
    s >> fixedModNum;
    for(int i = 0; i < fixedModNum; i++) {
        getline(input, line);
        stringstream s(line);
        FixedModule *fm = new FixedModule();
        s >> null;
        s >> fm->name;
        s >> fm->x;
        s >> fm->y;
        s >> fm->width;
        s >> fm->height;
        fm->type = "fixed";
        int fmL = fm->x;
        int fmR = fm->x + fm->width;
        int fmB = fm->y;
        int fmT = fm->y + fm->height;
        fixedModules.push_back(fm) ;
        fixedModulesMap[fm->name] = fm;
        // cout << fm->name << " " << fm->x << " " << fm->y << " " << fm->width << " " << fm->height << endl;
    }
    sort(fixedModules.begin(), fixedModules.end(), compareFixed);
}

void readNet(vector<Net*> &nets) {
    string line;
    string null;
    int netNum;
    getline(input, line); //read blank line
    getline(input, line);
    stringstream s(line);
    s >> null;
    s >> netNum;
    for(int i = 0; i < netNum; i++) {
        getline(input, line);
        stringstream s(line);
        Net *n = new Net();
        s >> null;
        s >> n->module1Name;
        s >> n->module2Name;
        s >> n->netWeight;
        nets.push_back(n);
        // cout << n->module1Name << " " << n->module2Name << " " << n->netWeight << endl;
    }
}

