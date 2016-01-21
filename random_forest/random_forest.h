#ifndef RANDOM_FOREST_H
#define RANDOM_FOREST_H
#include "desion_tree.h"
#include "thread_pool.h"

namespace lancelot {
    class random_forest{
    public:
        char test(int* point, int dataLen);
        void train();

    public:
        random_forest(int treeNum, const char* fileName);
        ~random_forest(){
            delete[] _trees;
        }

    private:
        random_forest(random_forest&){}
        random_forest&operator=(random_forest&){return *this;}
        random_forest(){}

    private:
        DesionTree* _trees;
        int _treeNum;
    };

}


#endif // RANDOM_FOREST_H
