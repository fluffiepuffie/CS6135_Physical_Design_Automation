#include "class.h"

int Module::B() const { cout << "no B()";}
int Module::T() const {cout << "no T()";}
int Module::L() const {cout << "no L()";}
int Module::R() const {cout << "no R()";}

pair<int, int> Module::getCenter() const {
    return make_pair((this->L() + this->R())/ 2, (this->B() + this->T())/2);
}


SoftModule::SoftModule(){}
SoftModule::SoftModule(const SoftModule& other){
    type = other.type;
    name = other.name;
    x = other.x;
    y = other.y;
    minSize = other.minSize;
    candidateWH = other.candidateWH;
    WHInd = other.WHInd;
}
bool SoftModule::compare(const SoftModule* self, const SoftModule* other) const{
    return (self->R() - self->L()) * (self->T() - self->B()) < (other->R() - other->L()) * (other->T() - other->B());
}
int SoftModule::T() const {return this->y + this->candidateWH[this->WHInd].second;}
int SoftModule::B() const {return this->y;}
int SoftModule::L() const {return this->x;}
int SoftModule::R() const {return this->x + this->candidateWH[this->WHInd].first;}


int FixedModule::T() const {return this->y + this->height;}
int FixedModule::B() const {return this->y;}
int FixedModule::L() const {return this->x;}
int FixedModule::R() const {return this->x + this->width;}

bool FixedModule::operator<(const FixedModule& other) const{
    if(this->x == other.x) {
        return this->y < other.y;
    }
    return this->x < other.x;
}