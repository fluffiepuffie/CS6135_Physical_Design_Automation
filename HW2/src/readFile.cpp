#include"class.h"
using namespace std;

std::string line;
std::vector<std::string> splitedLines;
extern ifstream input;

void readTech(unordered_map<string, Tech*>& techs) {
    getline(input, line);
	boost::split(splitedLines, line, boost::is_any_of(" "));
    int NumTechs = stoi(splitedLines[1]);
    for(int i = 0; i < NumTechs; i++) {
        Tech *t = new Tech();
        getline(input, line);
        boost::split(splitedLines, line, boost::is_any_of(" "));
        int LibCellNum = stol(splitedLines[2]);
        string TechName = splitedLines[1];
        t->config(TechName);
        for(int j = 0; j < LibCellNum; j++) {
            getline(input, line);
            boost::split(splitedLines, line, boost::is_any_of(" "));
            // cout<<line<<endl;
            LibCell *LC = new LibCell();
            LC->config(splitedLines[1], stoi(splitedLines[2]) * stoi(splitedLines[3]));
            t->addLibCell(*LC);
            
        }
        techs.insert({t->name, t});
    }
    
}
void testReadTech(unordered_map<string, Tech*> &techs){
    for(auto const &tech: techs) {
        cout<<tech.second->name<<" "<<endl;
        for(auto const &lc: tech.second->libCells) {
            cout<<lc.second.size;
            cout<<endl;

        }
        cout<<endl;
    }
}

void readDie(unordered_map<string, Die*> &dies) {
    getline(input, line); //read blank line
    getline(input, line);
    boost::split(splitedLines, line, boost::is_any_of(" "));
    unsigned long long dieWidth = stoul(splitedLines[1]);
    unsigned long long dieHeight = stoul(splitedLines[2]);
    for(int i = 0; i < 2; i++) {
        Die *d = new Die();
        getline(input, line);
        boost::split(splitedLines, line, boost::is_any_of(" "));
        d->config(splitedLines[0], splitedLines[1], dieWidth*dieHeight, stoi(splitedLines[2]));
        dies[splitedLines[0]] = d;
    }
}

void testReadDie(unordered_map<string, Die> &dies){
    for(auto const &die: dies) {
        cout<<die.second.name<<" "<<endl;
        cout<<die.second.name<<" "; 
        cout<<die.second.techName<<" "; 
        cout<<die.second.maxSize<<" "; 
        cout<<die.second.maxUtilization<<" "; 
        cout<<endl;
        // for(auto const &d: die.second) {
        //     cout<<d.name<<" "; 
        //     cout<<d.techName<<" "; 
        //     cout<<d.area<<" "; 
        //     cout<<d.maxUtilization<<" "; 
        //     cout<<endl;
        // }
        cout<<endl;
    }
}

void readCell(unordered_map<string, Cell*> &cells) {
    
    getline(input, line); //read blank lin
    getline(input, line);
    boost::split(splitedLines, line, boost::is_any_of(" "));
    int NumCells = stoi(splitedLines[1]);
    for(int i = 0; i < NumCells; i++) {
        getline(input, line);
        // cout<<line<<endl;
        boost::split(splitedLines, line, boost::is_any_of(" "));
        string cellName = splitedLines[1];
        boost::split(splitedLines, splitedLines[2], boost::is_any_of("\r")); //erase char /r in the last word
        string libCellName = splitedLines[0];
        Cell *c = new Cell();
        c->config(cellName, libCellName);
        cells[cellName] = c;
    }
}

void testReadCell(unordered_map<string, Cell*> &cells) {
    for(auto const &cell: cells) {
        cout<<cell.second->name<<" ";
        cout<<cell.second->libCellName<<endl;
    }
    
}
void readNets(unordered_map<string, Cell*> &Cells, unordered_map<string, Net> &Nets) {
    getline(input, line); //read blank lin
    getline(input, line);
    boost::split(splitedLines, line, boost::is_any_of(" "));
    int NumNets = stoi(splitedLines[1]);
    for(int i = 0; i < NumNets; i++) {
        getline(input, line);
        boost::split(splitedLines, line, boost::is_any_of(" "));
        Net N;
        int numConnectedCells = stoi(splitedLines[2]);
        N.config(splitedLines[1], numConnectedCells);
        for(int j = 0; j < numConnectedCells; j++){
            getline(input, line);
            boost::split(splitedLines, line, boost::is_any_of(" "));
            boost::split(splitedLines, splitedLines[1], boost::is_any_of("\r"));
            // for(char c: splitedLines[0]) {
            //     cout<< static_cast<int>(c) <<endl;
            // }
            N.addConnectedCell(splitedLines[0]);
            Cells[splitedLines[0]]->addConnectedNet(N.name);
            //remember to add cell array
        }
        Nets[N.name] = N;
    }
}

void testReadNet(unordered_map<string, Cell*> &cells, unordered_map<string, Net> &Nets) {
    cout<<"cell Array:"<<endl;
    for(auto const &cell: cells) {
        cout<<cell.second->name<<": ";
        // for(int i = 0; i < cell.second.connectedNets.size(); i++) {
        //     cout<<cell.second.connectedNets[i];
        // }
        for(auto s: cell.second->connectedNets) {
            // cout<<(s == "N1")<< " ";
            cout<<s<< " ";
        }
        cout<<endl;
    }
    cout<<"net Array"<<endl;
    for(auto const &net: Nets) {
        cout<<net.second.name<<": ";
        // for(int i = 0; i < net.second.connectedCells.size(); i++) {
        //     cout<<net.second.connectedCells[i];
        // }
        for(auto s: net.second.connectedCells) {
            // cout<<(s == "C1")<< " ";
            cout<<s<<" ";//efwj
        }
        cout<<endl;
    }
    
}