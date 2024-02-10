#include "class.h"
#include "floorplan.h"

ifstream input;
vector<FixedModule *> fixedModules = {};
unordered_map<string, FixedModule*> fixedModulesMap;  
unordered_map<string, int> softModulesMap; 
vector<SoftModule *> softModules = {};
vector<Net *> nets = {};
vector<Module> modules = {};
Chip chip;
auto programStart = std::chrono::high_resolution_clock::now();

int main(int argc, char *argv[]) {
    input.open(argv[1]);
    // input.open("../testcase/public2.txt");
    readChip(chip);
    readSoftModule();
    readFixedModule();
    readNet(nets);
    auto inputEnd = std::chrono::high_resolution_clock::now();
    auto inputDur = std::chrono::duration_cast<std::chrono::milliseconds> (inputEnd - programStart);
    cout << " End Input: " << inputDur.count();
    floorplan fp;
    // for(int i = 0; i < fixedModules.size(); i++) {
    //     cout << fixedModules[i]->name << endl;
    // }
    fp.initFloorPlan();
    auto initialFloorplanEnd = std::chrono::high_resolution_clock::now();
    auto initialFloorplanDur = std::chrono::duration_cast<std::chrono::milliseconds> (initialFloorplanEnd - inputEnd);
    cout << " End Initial floorplan " << initialFloorplanDur.count() << endl;
    // fp.sa(0.95, 0.1, 0.85, 20);
    fp.perturb();
    auto findBestEnd = std::chrono::high_resolution_clock::now();
    auto findBestDur = std::chrono::duration_cast<std::chrono::milliseconds> (findBestEnd - initialFloorplanEnd);
    cout << " End findBest floorplan " << findBestDur.count() << endl;
    fp.writeFile(argv[2], fp.current);
    auto writeFileEnd = std::chrono::high_resolution_clock::now();
    auto writeFileDur = std::chrono::duration_cast<std::chrono::milliseconds> (writeFileEnd - findBestEnd);
    cout << " End writFile " << writeFileDur.count() << endl;
    for(int i = 0; i < fp.best.size(); i++) {
        cout << fp.best[i].name << " left: " << fp.best[i].L() << " bottom: " << fp.best[i].B() << " top: " << fp.best[i].T() << " right: " << fp.best[i].R()<< endl;
    }
}