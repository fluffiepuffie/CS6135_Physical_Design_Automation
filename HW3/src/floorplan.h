#ifndef _FLOORPLAN_H_
#define _FLORRPLAN_H_

#include "class.h"
extern vector<FixedModule *> fixedModules;
class floorplan{
    public: 
        vector<SoftModule> current, best, next;
        vector<Module *> isPlaced;
        vector<Module *> allModule;
        floorplan();
        void initFloorPlan();
        void initFloorPlan2(SoftModule *sm);
        bool isOverlapped(SoftModule const &softModules, int checkedModule);
        void Rshifting(SoftModule &softModules, int checkedModule);
        void Ushifting(SoftModule &softModuels, int checkedModule);
        long long getHPWL(vector<SoftModule> &softModules);
        void perturb();
        bool rotate();
        bool swap();
        bool move();
        bool slide();
        bool checkViolation1(SoftModule const &) ;
        bool checkViolation2(SoftModule const &, SoftModule const &) ;
        void writeFile(string outputFile, vector<SoftModule> const & softModules);
        void sa(long double P, long double frozen, long double decade, int k);
        long double cost(vector<SoftModule> &);

};


#endif