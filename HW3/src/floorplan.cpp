#include "floorplan.h"
#include "class.h"

extern vector<FixedModule *> fixedModules;
extern vector<SoftModule *> softModules;
extern unordered_map<string, FixedModule*> fixedModulesMap;
extern unordered_map<string, int> softModulesMap;
extern Chip chip;
extern vector<Net*> nets;
extern std::chrono::time_point<std::chrono::high_resolution_clock> programStart;

floorplan:: floorplan() {
    this->allModule.clear();
    this->current.clear();
    this->best.clear();
    this->next.clear();
    this->isPlaced.clear();
    for(int i = 0; i < softModules.size(); i++) {
        SoftModule *orig = new SoftModule();
        *orig = *softModules[i];
        this->allModule.push_back(orig);
        this->current.push_back(*orig);
    }
    for(int i = 0; i < fixedModules.size(); i++) {
        FixedModule *orig = new FixedModule();
        *orig = *fixedModules[i];
        this->allModule.push_back(orig);
        this->isPlaced.push_back(orig);
    }
    cout << this->isPlaced.size() << endl;
}


bool sortIsPlace (const Module *self, const Module  *other) {
    return self->y < other->y;
}

void floorplan::initFloorPlan() {
    cout << "Debug: start initFloorPlan()\n";
    for(SoftModule &sm: this->current) {
        // cout << sm.name << " " << sm.candidateWH[0].first * sm.candidateWH[0].second << endl;
        int j = 0;
        while(j < isPlaced.size()) {
            // cout << sm.name << endl;
            // cout << i << endl;
            // cout << sm.name << endl;
            // cout << isPlaced[i]->name << endl;
            // cout << sm.L() << " " << sm.B() << " " << sm.T() << " " << sm.R() << endl;
            if(isOverlapped(sm, j)) {
            // cout << sm.name << " versus " << isPlaced[j]->name << endl;
                // cout << isPlaced[i]->name << endl;
                if(sm.WHInd < sm.candidateWH.size() - 1) {
                    sm.WHInd++;
                }
                else {
                    sm.WHInd = 0;
                    Rshifting(sm, j);
                }
                j = 0;
                continue;
            }
            if(sm.L() >= chip.width){
                // Ushifting(sm, j);
                sm.y++;
                sm.x = 0;
                j = 0;
                continue;
            }
            else if(sm.R() > chip.width) {
                for(int i = 0; i < sm.candidateWH.size(); i++) {
                    sm.WHInd = i;
                    if(sm.R() <= chip.width) {
                        break;
                    }
                }
                if(sm.R() > chip.width) {
                    sm.WHInd = 0;
                    sm.y++;
                    sm.x = 0;
                }
                j = 0;
                continue;
            }
            j++;
        }
        // cout << this->isPlaced.size() << endl;
        isPlaced.push_back(&sm);
        // cout << this->isPlaced.size() << endl;
        sm.placed = true;
        sort(isPlaced.begin(), isPlaced.end(), sortIsPlace);
        // cout << sm.name << " " << sm.x << " " << sm.y << endl;
    }
    this->best = this->current;
    // for(int i = 0; i < isPlaced.size(); i++) {
    //     cout << isPlaced[i]->name << endl;
    // }
    // for(int i = 0; i < fixedModules.size(); i++) {
    //     cout << fixedModules[i]->name << " left: " << fixedModules[i]->L() << " bottom: " << fixedModules[i]->B() << " top: " << fixedModules[i]->T() << " right: " << fixedModules[i]->R()<< endl;
    // }
    // for(int i = 0; i < this->current.size(); i++) {
    //     cout << this->current[i].name << " left: " << this->current[i].L() << " bottom: " << this->current[i].B() << " top: " << this->current[i].T() << " right: " << this->current[i].R()<< endl;
    // }
    cout << "Debug: end initFloorPlan()" << endl;
}


bool floorplan::isOverlapped(SoftModule const &sm, int checked) {
    if(sm.L() >= this->isPlaced[checked]->R() || sm.R() <= this->isPlaced[checked]->L()) {
        // cout << "here1" << endl;
        return false;
    }
    if(sm.B() >= isPlaced[checked]->T() || sm.T() <= isPlaced[checked]->B()) {
        // cout << "here2" << endl;
        return false;
    }
    // cout << "M11 overlap with CPU 0" << endl;
    return true;
}

void floorplan::Rshifting(SoftModule &sm, int checked) {
    sm.x = this->isPlaced[checked]->R();
}

void floorplan::Ushifting(SoftModule &sm, int checked) {
    cout << sm.name << isPlaced[checked]->name << endl;
    // cout << checked << endl;
    sm.y = this->isPlaced[checked]->T();
}

long long floorplan::getHPWL(vector<SoftModule> &softModules)  {
    long long totalL = 0;
    for(auto *n:nets) {
        long long temp = 0;
        Module *m1;
        Module *m2;
        // cout << n->module1Name << endl;
        // cout << n->module2Name << endl;
        if(fixedModulesMap.count(n->module1Name)) {
            m1 = fixedModulesMap[n->module1Name];
            m2 = &softModules[softModulesMap[n->module2Name]];
        }
        else if(fixedModulesMap.count(n->module2Name)) {
            m1 = fixedModulesMap[n->module2Name];
            m2 = &softModules[softModulesMap[n->module1Name]];
        }
        else {
            m1 = &softModules[softModulesMap[n->module1Name]];
            m2 = &softModules[softModulesMap[n->module2Name]];
        }
        // cout << "m1 " << m1->name << " "  << m1->L() << " " << m1->B() << " " << m1->T() << " " << m1->R() << " " << m1->getCenter().first  << " " << m1->getCenter().second << endl;
        // cout << "m2 " << m2->name << " "  << m2->L() << " " << m2->B() << " " << m2->T() << " " << m2->R() << " " << m2->getCenter().first  << " " << m2->getCenter().second << endl;
        temp += abs(m1->getCenter().first - m2->getCenter().first);
        temp += abs(m1->getCenter().second - m2->getCenter().second);
        temp *= n->netWeight;
        // cout << "temp " << temp << endl;
        totalL += temp;
    }
    return totalL;
}

bool floorplan::rotate() {
    int rSm = rand() % this->next.size();
    int rWHInd = 0;
    do {
        rWHInd = rand() % this->next[rSm].candidateWH.size();
    } while(rWHInd == this->next[rSm].WHInd && this->next[rSm].candidateWH.size() > 1);
    // cout << " rotate: " << this->next[rSm].name << endl;
    this->next[rSm].WHInd = rWHInd;
    return checkViolation1(this->next[rSm]);
}

bool floorplan::swap() {
    int rSm1 = rand() % this->next.size();
    int rSm2;
    do{
        rSm2 = rand() % this->next.size();
    }while(rSm1 == rSm2 && this->next.size() > 1);
    int rSm1x = this->next[rSm1].x, rSm1y = this->next[rSm1].y;
    int rSm2x = this->next[rSm2].x, rSm2y = this->next[rSm2].y;
    // cout << " swap: " << this->next[rSm1].name << " with " << this->next[rSm2].name << endl;
    this->next[rSm1].x = rSm2x; this->next[rSm1].y = rSm2y;
    this->next[rSm2].x = rSm1x; this->next[rSm2].y = rSm1y;
    
    return checkViolation2(this->next[rSm1], this->next[rSm2]);
}

bool floorplan::move() {
    int rSm = rand() % this->next.size();
    int rx = rand() % chip.width;
    int ry = rand() % chip.height;
    // cout << "random move " << this->next[rSm].name << " " << this->next[rSm].x << " " << this->next[rSm].y << " to " << rx << " " << ry << endl;
    this->next[rSm].x = rx; this->next[rSm].y = ry;
    for(int i = 0; i < this->next[rSm].candidateWH.size() && !checkViolation1(this->next[rSm]); i++) {
        // cout << " Ive tried" << endl;
        this->next[rSm].WHInd = i;
    }
    return checkViolation1(this->next[rSm]);
}

bool floorplan::slide() {
    int rSm = rand() % this->next.size();
    int rDir = (rand() % 2);
    int rMv = rand() % 5 + 1;
    if(rDir == 0) {
        int rOp = rand() % 2;
        if(rOp == 0) {
            this->next[rSm].x -= rMv;
        } else{
            this->next[rSm].x += rMv;
        }
    }
    else {
        int rOp = rand() % 2;
        if(rOp == 0) {
            this->next[rSm].y -= rMv;
        } else {
            this->next[rSm].y += rMv;
        }
    }
    for(int i = 0; i < this->next[rSm].candidateWH.size() && !checkViolation1(this->next[rSm]); i++) {
        // cout << " Ive tried" << endl;
        this->next[rSm].WHInd = i;
    }
    return checkViolation1(this->next[rSm]);
}

bool floorplan::checkViolation2(SoftModule const &sm1, SoftModule const &sm2)  {
    if(sm1.R() > chip.width || sm1.T() > chip.height || sm1.L() > chip.width || sm1.B() > chip.height){
        return false;
    }
    if(sm1.R() < 0 || sm1.T() < 0 || sm1.L()  < 0 || sm1.B() < 0){
        return false;
    }

    if(sm2.R() > chip.width || sm2.T() > chip.height || sm2.L() > chip.width || sm2.B() > chip.height){
        return false;
    }
    if(sm2.R() < 0 || sm2.T() < 0 || sm2.L()  < 0 || sm2.B() < 0){
        return false;
    }

    for(int i = 0; i < isPlaced.size(); i++) {
        if(isPlaced[i]->name == sm1.name){
            continue;
        } 
        else {
            if(isOverlapped(sm1, i)) {
                return false;
            }
        }
    }

    for(int i = 0; i < isPlaced.size(); i++) {
        if(isPlaced[i]->name == sm2.name){
            continue;
        } 
        else {
            if(isOverlapped(sm2, i)) {
                return false;
            }
        }
    }
    // cout << sm1.name << " swap with " << sm2.name << " is legal " << endl;
    return true;

}

bool floorplan::checkViolation1(SoftModule const &sm1)  {
    
    if(sm1.R() > chip.width || sm1.T() > chip.height || sm1.L() > chip.width || sm1.B() > chip.height){
        return false;
    }
    if(sm1.R() < 0 || sm1.T() < 0 || sm1.L()  < 0 || sm1.B() < 0){
        return false;
    }
    for(int i = 0; i < isPlaced.size(); i++) {
        if(isPlaced[i]->name == sm1.name){
            continue;
        } 
        else {
            if(isOverlapped(sm1, i)) {
                return false;
            }
        }
    }
    // cout << sm1.name << " move/slide/rotate is legal " << endl; 
    return true;

}

// long double floorplan::cost(vector<SoftModule> &softModules) {
//     this->getHPWL(softModules);
// }

void floorplan::sa(long double P, long double frozen, long double decade, int k) {
    long double Temp = 0;
    long double deltaAV = 0;
    int legalMove = 0;
    for(int i = 0; i < 10000; i++) {
        next = current;
        int r = rand() % 4;
        bool islegal = false;
        if(r == 0) {
            islegal = rotate();
        } else if(r == 1) {
            islegal = swap();
        } else if(r == 2) {
            islegal = slide();
        } else {
            islegal = move();
        }
        if(islegal == true && getHPWL(next) > getHPWL(current)) {
            legalMove++;
            deltaAV += getHPWL(next) - getHPWL(current);
        }
    }
    deltaAV /= legalMove;
    Temp = -deltaAV  /log(0.95);
    best = current;
    next = current;
    int M = 0, MT = 0, uphill = 0, reject = 0, N = k * next.size();
    do{
        MT = 0, uphill = 0, reject = 0;
        do {
            bool islegal = false;
            M = rand() % 4;
            if(M == 0) {
                islegal = rotate();
            } else if(M == 1) {
                islegal = swap();
            } else if(M == 2) {
                islegal = slide();
            } else if(M == 3) {
                islegal = move();
            }
            MT++;
            long double deltaCost = getHPWL(next) - getHPWL(current);
            if((deltaCost < 0 || rand() % 101 < deltaCost * 100) && islegal) {
                if(deltaCost > 0) {
                    uphill++;
                }
                current = next;
                if(getHPWL(next) < getHPWL(best)) {
                    best = next;
                }
            }
            else {
                reject++;
            }

        }while(uphill < N && MT < 2*N);
        Temp = Temp * decade;
        // cout << Temp << endl;
    }while(reject/MT < 0.95 && Temp > 0.1);
}

void floorplan::perturb () {
    cout << "Debug: start perturb() " << endl;
    while(true){
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds> (end - programStart);
        if(duration.count() > 590000)
            break;
        // cout << "duration: " << duration.count() << endl;
        bool islegal = false;
        this->next = this->current;
        int r = rand() % 100;
        // int r = 2;
        if(r >= 0 && r <= 29){
            islegal = rotate();
        }
        else if(r >= 30 && r <= 49) {
            islegal = swap();
        }
        else if(r >= 50 && r <= 79) {
            islegal = slide();
        }
        else if(r > 79) {
            islegal = move();
        }

        if(islegal) {
            // cout << "is legal" << endl;
            this->current = this->getHPWL(this->next) < this->getHPWL(this->current) ? this->next : this->current;
            this->best = this->getHPWL(this->next) < this->getHPWL(this->best) ? this->next : this->best;
        }
    }
    // this->best = this->current;
    cout << "Debug: end perturb() " << endl;

}

void floorplan::writeFile(string outputFile, vector<SoftModule> const & softModules) {
    ofstream output;
    output.open(outputFile);
    if(output.is_open()) {
        output << "Wirelength " << getHPWL(this->current) << endl << endl;
        output << "NumSoftModules " << softModules.size() << endl;
        for(int i = 0; i < softModules.size(); i++) {
            output << softModules[i].name << " " << softModules[i].x << " " << softModules[i].y << " " << softModules[i].candidateWH[softModules[i].WHInd].first << " " << softModules[i].candidateWH[softModules[i].WHInd].second << endl;
        }
    }
}
// bool floorplan::checkOverlap2(int sm, FixedModule const *fm) {
//     if(fm->R() >softModules[sm].L()) {
//         // cout << softModules[sm]->name << endl;
//         // cout << fm->name << endl;
//         if(softModules[sm].L() >= fm->R() || softModules[sm].R() <= fm->L())
//             return false;
//         if(softModules[sm].B() >= fm->T() || softModules[sm].T() <= fm->B())
//             return false;
//         return true;
//     }

// }