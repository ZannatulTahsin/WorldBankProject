#ifndef treenode_hpp
#define treenode_hpp
#include <string>
#include <iostream>
#include "country.h"
#include "Node.h"
#include "timeseries2.h"
#include "array.h"


class treenode {
    public:
        //data 
        TimeSeries* countries[512];
         double min; double max; double midpoint; int countrycount;
        //BST
         treenode* left; 
         treenode* right;


        //Constructor
         treenode(){left = nullptr; right = nullptr;
        for (int i = 0; i < 512; i++){
            countries[i] = nullptr;
        }
        min = 0; max = 0; midpoint = 0;countrycount = 0;
    };
        
    //Destructor
    ~treenode(){
            delete left; delete right;
            for (int i = 0; i<512; i++){
                countries[i] = nullptr;
            }
        }
};

#endif