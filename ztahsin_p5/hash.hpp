#ifndef hash_hpp
#define hash_hpp
#include <string>
#include "country.h"

class hash {
    public:
    //Keep track of occupied spaces in removal and search 
    bool isOcc = false;
    //Country linked list objects  
    country* countries; 
    int ind = 0;

    //constructor
    hash(){
        countries = nullptr;
        isOcc = false; 
    }
    //destructor
    ~hash(){
        delete countries;
        countries = nullptr; 
        isOcc = false;
        ind = 0;
    }
       
};

#endif