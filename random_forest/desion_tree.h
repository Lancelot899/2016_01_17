#ifndef DESION_TREE_H
#define DESION_TREE_H
#include <stddef.h>

namespace lancelot {
    struct __tree{
        int feature;
        int val;
        __tree* _left;
        __tree* _right;
        __tree():feature(-1), val(-1), _left(nullptr), _right(nullptr){}
        ~__tree(){
            if(_left != nullptr)
                delete _left;
            if(_right != nullptr)
                delete _right;
        }
    };

    class DesionTree{
    public:
        DesionTree():_desion_tree(nullptr){}

        DesionTree(const char *path);
        ~DesionTree(){
            freeTraData();
            delete _desion_tree;
        }

        void createTree(int treeHeight);

        const int *loadData(const char *path);
        char test(int* data, int dataLen);

    private:
        struct __dataSet{
            int*  _data;
            int   _len;
            char* _labels;
            int   dim;
            __dataSet()
                :_data(nullptr), _len(0),dim(0),
                  _labels(nullptr){}
            ~__dataSet(){
                if(_data != nullptr)
                    delete[]_data;
                if(_labels != nullptr)
                    delete[]_labels;
            }

            int& at(int i, int j){
                return _data[i * dim + j];
            }

        };


    private:
        DesionTree(DesionTree&){}
        DesionTree(DesionTree&&){}
        DesionTree&operator=(DesionTree&){return *this;}
        void freeTraData(){
            if(_dataSet._data != nullptr){
                delete[]_dataSet._data;
                _dataSet._data = nullptr;
            }
            if(_dataSet._labels != nullptr){
                delete[]_dataSet._labels;
                _dataSet._labels = nullptr;
            }
        }

        bool chooseBestFeature(__dataSet& dataSet, int& bestfeature, int& bestvalue, __dataSet &left, __dataSet &right);
        bool binSplitDataSet(__dataSet&dataSet, int feature, int value,__dataSet& left, __dataSet& right);
        double impurity(__dataSet& dataSet);
        __tree *_createTree(__dataSet& dataSet, int curHeight, int treeHeight, double err = 0.0001);

    private:
        __dataSet   _dataSet;
        __tree*     _desion_tree;

    };



}


#endif // DESION_TREE_H
