#include "class.h"
#include <time.h>
using namespace std;

std::ifstream input;
std::ofstream output;
int main(int argc, char** argv) {
   double iterationStart = (double) clock() / CLOCKS_PER_SEC;
   double iterationUsedTime;
   double currentTime;
   input.open(argv[1]);
   // unordered_map<string, Die> dies = {};
   unordered_map<string, Die*> dies = {}; //modified 
   unordered_map<string, Tech*> techs = {};
   unordered_map<string, Cell*> cells = {};
   unordered_map<string, Net> nets = {};
   vector<unordered_map<string, Die*>> backup = {};
   vector<long> gains = {0};
   readTech(techs);
   // testReadTech(techs);
   readDie(dies);
   // testReadDie(dies);
   readCell(cells);
   // testReadCell(cells);
   readNets(cells, nets);
   // testReadNet(cells, nets);
   input.close();
   long maxPrefixSumIndex = 0;
   long maxPrefixSum = 0;
   // cout << "start intial solution" << endl;
   testSolution(techs, dies, cells, nets);
   int iteration = 0;
   int deadLock = 1;
   // cout << dies["DieA"]->maxSize * dies["DieA"]->maxUtilization << endl;
   // cout << dies["DieA"]->usedSize * 100<< endl;
   // cout << dies["DieB"]->maxSize * dies["DieB"]->maxUtilization << endl;
   // cout << dies["DieB"]->usedSize * 100<< endl;
   do {
      iteration++;
      // cout << "iteration: " << iteration << endl;
      gains = {0};
      maxPrefixSumIndex = 0; 
      maxPrefixSum = 0;
      computeInitialGain(dies, cells, nets);

      double moveAndUpdateStart = (double) clock() / CLOCKS_PER_SEC;
      for(int i = 0; i < cells.size(); i++) {
         if(currentTime > 295) 
            break;
         moveAndUpdate(techs, dies, cells, nets, gains, backup);
         double moveAndUpdateOver = (double) clock() / CLOCKS_PER_SEC;
         double moveAndUpdateUsedTime = moveAndUpdateOver - moveAndUpdateStart;
         currentTime = moveAndUpdateOver - iterationStart;
         if(maxPrefixSum - maxPrefixSum / 10 >= gains.back() || currentTime > 290)  
            break;
         // if(iteration >= 0 && iteration <= 4) {
         //    if(maxPrefixSum - maxPrefixSum / 10 >= gains.back() || moveAndUpdateUsedTime > 40)
         //       break;
         // }
         // else{
         //    if(maxPrefixSum - maxPrefixSum / 10 >= gains.back() || moveAndUpdateUsedTime > 10 || currentTime > 290)  
         //       break;
         // }
         maxPrefixSumIndex = (gains.back() > maxPrefixSum) ?  i+1 : maxPrefixSumIndex;
         maxPrefixSum = (gains.back() > maxPrefixSum) ? gains.back() : maxPrefixSum;
         deadLock = gains[maxPrefixSumIndex] == 1 ? deadLock + 1 : deadLock;
      }
      // cout << currentTime << endl;
      if(currentTime > 290)
         break;
      if(deadLock == 5)
         break;
      
      // for(auto gain :gains) {
      //    cout<<gain <<" " ;
      // }
      // cout<<"gains size: " << gains.size()<<endl;
      // cout<< "maxPrefixSumIndex: " << maxPrefixSumIndex <<endl;
      dies = backup[maxPrefixSumIndex];
      // cout << "start reconfig Cell and Net" << endl;
      reconfigCellAndNet(dies, cells, nets);
      backup.clear();
   } while(gains[maxPrefixSumIndex] > 0 && iteration < 1000);

   // write file
   output.open(argv[2]);
   if(output.is_open()) {
      long cutsize = 0;
      for(auto &net : nets) {
         if(net.second.connectedCellsOnDieA.size() > 0 && net.second.connectedCellsOnDieB.size() > 0) {
            cutsize++;
         }
      }
      output << "CutSize " << cutsize << '\n';
      output << "DieA " << dies["DieA"]->assignedCells.size() << '\n';
      for(string cell : dies["DieA"]->assignedCells) {
         output << cell <<'\n' ;
      }
      output << "DieB " << dies["DieB"]->assignedCells.size() << '\n';
      for(string cell : dies["DieB"]->assignedCells) {
         output << cell <<'\n' ;
      }
   }
   output.close();
}