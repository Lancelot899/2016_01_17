#include "random_forest.h"

using namespace lancelot;

random_forest::random_forest(int treeNum, const char* fileName):_treeNum(treeNum){
    _trees = new DesionTree[_treeNum];
    for(int i(0); i < _treeNum; i++)
        _trees[i].loadData(fileName);
}

char random_forest::test(int *point, int dataLen){

}

void random_forest::train(){

}

