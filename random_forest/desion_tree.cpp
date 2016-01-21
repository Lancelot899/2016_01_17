#include "desion_tree.h"
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <string.h>
#include <set>

using namespace lancelot;


const int* DesionTree::loadData(const char *path){
    //! load data
    FILE *fr = NULL;
    fr = fopen(path, "r");
    if(NULL == fr){
        return nullptr;
    }
    int fileLen(0);
    fseek(fr, 0, SEEK_END);
    fileLen = ftell(fr);
    fseek(fr, 0, SEEK_SET);
    char* fileData = new char[fileLen];
    memset(fileData, 0, fileLen);
    fread(fileData, fileLen, 1, fr);
    fclose(fr);

    //! ask for memery
    char* p1(fileData), *p2(fileData);
    int cnt(0);
    while(*p1 != '\0'){
        if(*p1 == '\n')
            ++cnt;
        ++p1;
    }
    _dataSet._len = cnt;
    _dataSet._labels = new char[_dataSet._len];
    p1 = p2;
    cnt = 0;
    while(*p1 != '\n'){
        if(*p1 == ',')
            ++cnt;
        ++p1;
    }
    p1 = p2;
    _dataSet.dim = cnt;
    _dataSet._data = new int[_dataSet._len * _dataSet.dim];
    memset(_dataSet._data, 0, sizeof(int) * _dataSet._len * _dataSet.dim);
    char*pStr    = _dataSet._labels;
    int *pData   = _dataSet._data;
    char pTmp;

    //! analysis data
    while(*p1 != 0){
        if(*p1 >= 'A' && *p1 <= 'Z'){
            *pStr = *p1;
            ++pStr;
        }
        p1++;
        if(*p1 == ','){
            p2 = (++p1) + 1;
            while(*p2 != ',' && *p2 != '\n' && *p2 != 0)++p2;
            pTmp = *p2;
            *p2 = 0;
            *(pData++) = atoi(p1);
            *p2 = pTmp;
            p1 = p2;
        }
    }

    delete[] fileData;
    return _dataSet._data;
}

DesionTree::DesionTree(const char* path):
    _desion_tree(nullptr){
    loadData(path);
}

char DesionTree::test(int *data, int dataLen){
    char res(0);
    createTree(5);
    return res;
}

void DesionTree::createTree(int treeHeight){
    if(_desion_tree != nullptr)
        delete _desion_tree;
    _desion_tree = _createTree(_dataSet, 0, treeHeight);
    freeTraData();
}

__tree* DesionTree::_createTree(__dataSet &dataSet, int curHeight, int treeHeight, double err){
    if(curHeight >= treeHeight)
        return nullptr;
    if(dataSet._len <= 1)
        return nullptr;
    double impur = impurity(dataSet);
    if(impur < err)
        return nullptr;
    int feature(-1);
    int value(-1);
    __dataSet left, right;
    if (!chooseBestFeature(dataSet, feature, value, left, right))
        return nullptr;
    __tree* tree = new __tree;
    tree->feature = feature;
    tree->val = value;
    printf("tree: %d:feature = %d, value = %d\n", curHeight,feature, value);
    tree->_left  = _createTree(left, curHeight + 1, treeHeight);
    tree->_right = _createTree(right, curHeight + 1, treeHeight);
    return tree;
}

bool DesionTree::chooseBestFeature(__dataSet &dataSet, int &bestfeature, int &bestvalue, __dataSet&left, __dataSet&right){
    if(dataSet._len <= 1)
        return false;
    bestfeature = 0;
    bestvalue = dataSet.at(0,0);
    double bestImpurity = 3.0;
    double curImpurity = -1.0;
    for(int j(0); j < dataSet.dim; j++){
        for(int i(0); i < dataSet._len; i++){
            printf("i:%d, j: %d\n", j, i);
            binSplitDataSet(dataSet,j, dataSet.at(i,j), left, right);
            curImpurity = impurity(left) + impurity(right);
            if(curImpurity < bestImpurity){
                bestfeature = j;
                bestvalue = dataSet.at(i, j);
                bestImpurity = curImpurity;
                printf("feature = %d, value = %d\n", bestfeature, bestvalue);
            }
            else{
                if(left._len > 0){
                    delete[]left._data;
                    delete[]left._labels;
                    left._data = nullptr;
                    left._labels = nullptr;
                }
                if(right._len > 0){
                    delete[]right._data;
                    delete[]right._labels;
                    right._data = nullptr;
                    right._labels = nullptr;
                }
            }

        }
    }
    return true;
}

bool DesionTree::binSplitDataSet(__dataSet&dataSet, int feature, int value, __dataSet &left, __dataSet &right){
    if(dataSet._len <= 1)
        return false;
    for(int i(0); i < dataSet._len; i++){
        if(dataSet.at(i, feature) <= value)
            ++left._len;
        else
            ++right._len;
    }
    if(left._len > 0){
        left._data = new int[left._len * (dataSet.dim - 1)];
        left._labels = new char[left._len];
        left.dim = dataSet.dim - 1;
    }
    if(right._len > 0){
        right._data = new int[right._len * (dataSet.dim - 1)];
        right._labels = new char[right._len];
        right.dim = dataSet.dim - 1;
    }
    int*  pld = left._data;
    int*  prd = right._data;
    char* plt = left._labels;
    char* prt = right._labels;
    for(int i(0); i < dataSet._len; i++){
        if(dataSet.at(i, feature) <= value){
            if(feature == dataSet.dim){
                memcpy(pld, &(dataSet._data[i]), feature - 1);
                pld += feature - 1;
            }
            else{
                if(feature > 0){
                    memcpy(pld, &(dataSet._data[i]), feature);
                    pld += feature;
                }
                memcpy(pld, &(dataSet._data[i]) + feature + 1, dataSet.dim - feature - 1);
                pld += dataSet.dim - feature - 1;

            }
            *plt = dataSet._labels[i];
            ++plt;
        }
        else{
            if(feature == 16){
                memcpy(prd, &(dataSet._data[i]), feature - 1);
                prd += feature - 1;
            }
            else{
                if(feature > 0){
                    memcpy(prd, &(dataSet._data[i]), feature);
                    prd += feature;
                }
                memcpy(prd, &(dataSet._data[i]) + feature + 1, dataSet.dim - feature - 1);

                prd += dataSet.dim - feature - 1;
            }
            *prt = dataSet._labels[i];
            ++prt;
        }
    }
    return true;

}

double DesionTree::impurity(__dataSet &dataSet){
    if(dataSet._len <= 1)
        return 0;
    std::set<char> labelSet;
    for(int i(0); i < dataSet._len; i++)
        labelSet.insert(dataSet._labels[i]);
    if(labelSet.size() == 1)
        return 0;
    double err(0.0);
    double cnt(0.0);
    for(auto i = labelSet.begin(); i != labelSet.end(); i++){
        cnt = 0.0;
        for(int j(0); j < dataSet._len; j++){
            if(dataSet._labels[j] == *i)
                cnt += 1.0;
        }
        cnt /= double(dataSet._len);
        cnt *= cnt;
        err += cnt;
    }
    return 1 - err;
}
