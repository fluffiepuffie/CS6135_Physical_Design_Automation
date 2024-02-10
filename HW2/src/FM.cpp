#include "class.h"
using namespace std;

void shuffleCells(vector<string> &allCells) {
    random_device seed;
    default_random_engine rng(seed());
    shuffle(allCells.begin(), allCells.end(), rng);
}

int checkSizeBeforeMove (Tech &tech, Die const& to, Cell const& cell) {

    // cout << tech.libCells[cell.libCellName].size << endl;
    if((to.usedSize + tech.libCells[cell.libCellName].size) * 100 <= to.maxSize * to.maxUtilization ) {
        // cout << to.usedSize + tech.libCells[cell.libCellName].size * 100 << endl;
        // cout << to.maxSize * to.maxUtilization << endl;

        return 1;
    }
    else 
        return 0;
}

void testSolution( unordered_map<string, Tech*> &techs, unordered_map<string, Die*> &dies, unordered_map<string, Cell*> &cells, unordered_map<string, Net> &nets) {
    Die *DieA = dies["DieA"];
    Die *DieB = dies["DieB"];
    Tech *TA = techs[DieA->techName];
    Tech *TB = techs[DieB->techName];
    //assign cell to DieA if the result of assignment is under constraint
    //otherwise, assign to DieB
    int maxPin = 0;
    for(auto &cell: cells) {
        Cell *c = cell.second;
        if(c->connectedNets.size() > maxPin)
            maxPin = c->connectedNets.size();
        // if(DieA->usedSize + TA->libCells[c->libCellName].size < static_cast<double>(DieA->maxUtilization) / 100 * DieA->maxSize ) {
        if((DieA->usedSize + TA->libCells[c->libCellName].size) * 100 <= DieA->maxUtilization * DieA->maxSize)  {
            DieA->assignedCells.insert(c->name);
            DieA->usedSize += TA->libCells[c->libCellName].size;
            //update cell
            c->assignedDie = "DieA";
            for(string n : c->connectedNets) {
                nets[n].connectedCellsOnDieA.insert(c->name);
                nets[n].freeCells.insert(c->name);
            }
        }
        else {
            DieB->assignedCells.insert(c->name);
            DieB->usedSize += TB->libCells[c->libCellName].size;
            //update cell
            c->assignedDie = "DieB" ;
            for(string n : c->connectedNets) {
                nets[n].connectedCellsOnDieB.insert(c->name);
                nets[n].freeCells.insert(c->name);
            }
        }
        // if(DieA->usedSize * DieB->maxSize * DieB->maxUtilization  <= DieB->usedSize * DieA->maxSize * DieA->maxUtilization ) {
        //     if(checkSizeBeforeMove(*TA, *DieA, *c))  {
        //         DieA->assignedCells.insert(c->name);
        //         DieA->usedSize += TA->libCells[c->libCellName].size;
        //         //update cell
        //         c->assignedDie = "DieA";
        //         for(string n : c->connectedNets) {
        //             nets[n].connectedCellsOnDieA.insert(c->name);
        //             nets[n].freeCells.insert(c->name);
        //         }
        //     }
        //     else {
        //         DieB->assignedCells.insert(c->name);
        //         DieB->usedSize += TB->libCells[c->libCellName].size;
        //         //update cell
        //         c->assignedDie = "DieB" ;
        //         for(string n : c->connectedNets) {
        //             nets[n].connectedCellsOnDieB.insert(c->name);
        //             nets[n].freeCells.insert(c->name);
        //         }
        //     }
        // }
        // else {
        //     if(checkSizeBeforeMove(*TB, *DieB, *c))  {
        //         DieB->assignedCells.insert(c->name);
        //         DieB->usedSize += TB->libCells[c->libCellName].size;
        //         //update cell
        //         c->assignedDie = "DieB";
        //         for(string n : c->connectedNets) {
        //             nets[n].connectedCellsOnDieB.insert(c->name);
        //             nets[n].freeCells.insert(c->name);
        //         }
        //     }
        //     else {
        //         DieA->assignedCells.insert(c->name);
        //         DieA->usedSize += TA->libCells[c->libCellName].size;
        //         //update cell
        //         c->assignedDie = "DieA" ;
        //         for(string n : c->connectedNets) {
        //             nets[n].connectedCellsOnDieA.insert(c->name);
        //             nets[n].freeCells.insert(c->name);
        //         }
        //     }
        // }
    }
    
     //get bucketList ready
    DieA->bucketList.resize(maxPin * 2 + 1);
    DieB->bucketList.resize(maxPin * 2 + 1);
    for(auto a : DieA->bucketList) {
        a.clear();
    }

    for(auto b : DieB->bucketList) {
        b.clear();
    }
} 


void reconfigCellAndNet(unordered_map<string, Die*> &dies, unordered_map<string, Cell*> &cells, unordered_map<string, Net> &nets) {
    for(auto &die: dies) {
        Die *d = die.second;
        d->bucketList.clear();
        d->maxGainIndex = 0;
        int maxPin = 0;
        // update cell
        for(string cell: d->assignedCells) {
            Cell *c = cells[cell];
            if(c->connectedNets.size() > maxPin)
                maxPin = c->connectedNets.size();
            c->assignedDie = d->name;
            c->gain = 0;
            c->locked = 0;
            for(string net: c->connectedNets) {
                Net &n = nets[net];
                n.freeCells.insert(c->name);
                if(d->name == "DieA") {
                    n.connectedCellsOnDieB.erase(c->name);
                    n.connectedCellsOnDieA.insert(c->name);
                }
                else {
                    n.connectedCellsOnDieA.erase(c->name);
                    n.connectedCellsOnDieB.insert(c->name);
                }
            }
        }
        d->bucketList.resize(maxPin * 2 + 1);
    }
    for(auto &net: nets) {
        Net &n = net.second;
        n.lockedCells.clear();
    }
}
void computeInitialGain(unordered_map<string, Die*>&dies, unordered_map<string, Cell*> &cells, unordered_map<string, Net> &nets) {
    
    for(auto &cell: cells) {
        Cell *c = cell.second;
        string from = c->assignedDie;
        string to = (from == "DieA") ? "DieB" : "DieA";
        for(auto net: c->connectedNets) {
            Net &n = nets[net];
            // update net with backup state
            if(from == "DieA") {
                if(n.connectedCellsOnDieA.size() == 1) {
                    c->gain++;
                }
                if(n.connectedCellsOnDieB.size() == 0) {
                    c->gain--;
                } 
            } 
            else if(from == "DieB") {
                if(n.connectedCellsOnDieB.size() == 1) {
                    c->gain++;
                }
                if(n.connectedCellsOnDieA.size() == 0) {
                    c->gain--;
                }
            }
            // else
            //     cout<<"error"<<endl;
        }
        int gainIndex = c->gain + dies[c->assignedDie]->bucketList.size()/2;
        //build bucketList here
        dies[c->assignedDie]->bucketList[gainIndex].push_back(c->name);
        //cell.bucketListPointer point to cells.gain address in bucketList
        c->bucketListPointer = prev(dies[c->assignedDie]->bucketList[gainIndex].end());
        
        //update maxGainPointer
        if(gainIndex > dies[c->assignedDie]->maxGainIndex) {
            dies[c->assignedDie]->maxGainIndex = gainIndex;
        }
    }
}

void testInitialGain(unordered_map<string, Cell*> &cells) {
    for(auto const &cell : cells) {
        Cell *c = cell.second;
        cout<<c->name + " " << c->gain<<endl;
    }
}

void testBucketList(unordered_map<string, Die*> &Dies) {
    for(auto const &die: Dies) {
        cout<<die.second->name<<endl;
        for(int i = 0; i < die.second->bucketList.size(); i++) {
            int g = i;
            g -= die.second->bucketList.size()/2;
            cout<<"gain "<< g << " " ;
            for(auto j = die.second->bucketList[i].begin(); j != die.second->bucketList[i].end(); j++) {
                cout<<*j<< " ";
            }
            cout<<endl;
        }
        cout<<"max gain index: " << die.second->maxGainIndex<<endl;
    }
}



void updateGainAndBlist(unordered_map<string, Die*> &dies, Cell &updatedCell, int increment) {
    if(increment == 1) {
        long indexBias  = dies[updatedCell.assignedDie]->bucketList.size() / 2;
        vector<list<string>> &operatedBlist = dies[updatedCell.assignedDie]->bucketList;
        long origBlistIndex = updatedCell.gain + indexBias;
        updatedCell.gain++;
        long newBlistIndex = updatedCell.gain + indexBias;
        // update bucketList
        operatedBlist[origBlistIndex].erase(updatedCell.bucketListPointer);
        operatedBlist[newBlistIndex].push_back(updatedCell.name);
        updatedCell.bucketListPointer = prev(operatedBlist[newBlistIndex].end());
        //update maxGainPointer
        if(newBlistIndex > dies[updatedCell.assignedDie]->maxGainIndex)
            dies[updatedCell.assignedDie]->maxGainIndex = newBlistIndex;
    }
    else if(increment == -1) {
        long indexBias  = dies[updatedCell.assignedDie]->bucketList.size() / 2;
        vector<list<string>> &operatedBlist = dies[updatedCell.assignedDie]->bucketList;
        long origBlistIndex = updatedCell.gain + indexBias;
        updatedCell.gain--;
        long newBlistIndex = updatedCell.gain + indexBias;
        // update bucketList
        operatedBlist[origBlistIndex].erase(updatedCell.bucketListPointer);
        operatedBlist[newBlistIndex].push_back(updatedCell.name);
        updatedCell.bucketListPointer = prev(operatedBlist[newBlistIndex].end());
        //update maxGainPointer
        if(newBlistIndex > dies[updatedCell.assignedDie]->maxGainIndex)
            dies[updatedCell.assignedDie]->maxGainIndex = newBlistIndex;
    }

}

void updateAndBackupBeforeMove(unordered_map<string, Die*> &dies, unordered_map<string,Cell*> &cells, unordered_map<string, Net> &nets, vector<unordered_map<string, Die*>> &backup, string movedCell, string from, string to) {
    Cell *cell = cells[movedCell];
    Die *f = dies[from];
    Die *t = dies[to];
    backup.push_back({{f->name, f}, {t->name, t}});
    cell->locked = 1;
    for(string net: cell->connectedNets) {
        Net &n = nets[net];
    //lock the base cell and complement its block;
        n.freeCells.erase(movedCell);
        n.lockedCells.insert(movedCell);
        if(f->name == "DieA") {
            //if T(n) = 0 then increment gains of all free cells on n
            if(n.connectedCellsOnDieB.size() == 0) {
                for(string fc: n.freeCells) {
                    updateGainAndBlist(dies, *cells[fc], 1);
                }
            }
            // elseif T(n) = 1 then decrement gain of the only T cell on n, if it is free;
            else if(n.connectedCellsOnDieB.size() == 1) {
                for(string fc: n.connectedCellsOnDieB) {
                    if(cells[fc]->locked == 0) {
                        updateGainAndBlist(dies, *cells[fc], -1);
                    }
                }
            }
        }
        else if(f->name == "DieB") {
            //if T(n) = 0 then increment gains of all free cells on n
            if(n.connectedCellsOnDieA.size() == 0) {
                for(string fc: n.freeCells) {
                    updateGainAndBlist(dies, *cells[fc], 1);
                }
            }
            // elseif T(n) = 1 then decrement gain of the only T cell on n, if it is free;
            else if(n.connectedCellsOnDieA.size() == 1) {
                for(string fc: n.connectedCellsOnDieA) {
                    if(cells[fc]->locked == 0) {
                        updateGainAndBlist(dies, *cells[fc], -1);
                    }
                }
            }
        }
    }
}

void moveCell (unordered_map<string, Tech*> &techs, unordered_map<string, Die*> &dies, unordered_map<string, Cell*> &cells, unordered_map<string, Net> &nets, string from, string to, string movedCell) {
    Die *f = dies[from];
    Die *t = dies[to];
    Cell *cell = cells[movedCell];
    // update dies 
    // from dies
    f->usedSize -= techs[f->techName]->libCells[cell->libCellName].size;
    f->assignedCells.erase(cell->name);
    f->bucketList[cell->gain + f->bucketList.size() / 2].erase(cell->bucketListPointer);
    // maxGainIndex is updated at updateAndBackupBeforeMove

    // to dies
    t->usedSize += techs[t->techName]->libCells[cell->libCellName].size;
    t->assignedCells.insert(cell->name);

    //update cell
    cell->assignedDie = t->name;
    // locked was updated in updateAndBackupBeforeMove

    //update net
    for(string n: cell->connectedNets) {
        // freeCells and lockedCells was updated in updateAndBackupBeforeMove()
        if(from == "DieA") {
            nets[n].connectedCellsOnDieA.erase(cell->name);
            nets[n].connectedCellsOnDieB.insert(cell->name);
        }
        else if(from == "DieB") {
            nets[n].connectedCellsOnDieB.erase(cell->name);
            nets[n].connectedCellsOnDieA.insert(cell->name);
        }
    }
}

void updateAfterMove(unordered_map<string, Die*> &dies, unordered_map<string,Cell*> &cells, unordered_map<string, Net> &nets, string movedCell, string from, string to) {
    Cell *cell = cells[movedCell];
    Die *f = dies[from];
    Die *t = dies[to];
    for(string net:cell->connectedNets) {
        Net &n = nets[net];
        if(f->name == "DieA") {
        // if F(n) == 0 then decrement gains of all free cells on n
            if(n.connectedCellsOnDieA.size() == 0) {
                for(string fc: n.freeCells) {
                    updateGainAndBlist(dies, *cells[fc],  -1);
                }
            }
            // else if F(n) == 1 then increment gain of the only F cell on n, if it is free;
            else if(n.connectedCellsOnDieA.size() == 1) {
                for(string fc: n.connectedCellsOnDieA) {
                    if(cells[fc]->locked == 0) {
                        updateGainAndBlist(dies, *cells[fc], 1);
                    }
                }
            }
        }
        else if(f->name == "DieB"){
        // if F(n) == 0 then decrement gains of all free cells on n
            if(n.connectedCellsOnDieB.size() == 0) {
                for(string fc: n.freeCells) {
                    updateGainAndBlist(dies, *cells[fc],  -1);
                }
            }
            // else if F(n) == 1 then increment gain of the only F cell on n, if it is free;
            else if(n.connectedCellsOnDieB.size() == 1) {
                for(string fc: n.connectedCellsOnDieB) {
                    if(cells[fc]->locked == 0) {
                        updateGainAndBlist(dies, *cells[fc], 1);
                    }
                }
            }

        }
    }
}

void moveAndUpdate (unordered_map<string, Tech*>&techs, unordered_map<string, Die*> &dies, unordered_map<string, Cell*> &cells, unordered_map<string, Net> &nets, vector<long> &gains, vector<unordered_map<string, Die*>> &backup) {
    // pick a cell with the biggest gain from the die with biggest utilization
    // double Autil = static_cast<double>(dies["DieA"].usedSize)/dies["DieA"].maxSize;
    // double Butil = static_cast<double>(dies["DieB"].usedSize)/dies["DieB"].maxSize;
    Die *from = (dies["DieA"]->maxGainIndex > dies["DieB"]->maxGainIndex) ? dies["DieA"] : dies["DieB"];
    Die *to = (from->name == "DieA") ? dies["DieB"] : dies["DieA"];
    // Die *from = (Autil > Butil) ? &dies["DieA"] : &dies["DieB"];
    // Die *to = (from->name == "DieA") ? &dies["DieB"] : &dies["DieA"];
    int gainFlag = 0;
    // from maxGainIndex iterate to 0 to find the solution that matches constraint
    for(int j = 0; j < 2; j++) {
        if(gainFlag == 1)
            break;
        for(int i = from->maxGainIndex; i >= 0; i--) {
        // if a cell is picked and every move is done, then break;
            if(gainFlag == 1)
                break;
            // if a list in bucketList has no element, go to next list;
            if(from->bucketList[i].size() == 0)
                continue;
            // itertate over list
            for(auto j = from->bucketList[i].begin(); j != from->bucketList[i].end(); j++) {
                if(checkSizeBeforeMove(*techs[to->techName], *to, *cells[*j])){
                    string movedCell = cells[*j]->name;
                    gainFlag = 1;
                    long gain = i - from->bucketList.size()/2;
                    // cout << "gain " << gain << endl;
                    gains.push_back(gain + gains.back());
                    updateAndBackupBeforeMove(dies, cells, nets, backup, movedCell, from->name, to->name);
                    moveCell(techs, dies, cells, nets, from->name, to->name, movedCell);
                    updateAfterMove(dies, cells, nets, movedCell, from->name, to->name);
                    break;
                }
                else {
                    continue;
                }
            }
        }
        if(gainFlag == 0) {
            from = (from->name == "DieA") ? dies["DieB"] : dies["DieA"];
            to = (from->name == "DieA") ? dies["DieB"] : dies["DieA"];
        }
    } 
}


