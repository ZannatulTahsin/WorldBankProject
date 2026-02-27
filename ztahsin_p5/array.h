#ifndef array_hpp
#define array_hpp
#include <string>
//#include "country.h"
#include "hash.hpp"

class Array {
    public:
    //Data
    hash* all[512];
    
    bool loadAllCountries(const std::string& filename);
    void listCountry(std::string cname);
    void range(std::string scode);
    int hasher(std::string ccode);
    int secondhash(std::string ccode);
    void lookup(std::string ccode);
    bool insert(std::string ccode, std::string filename);

    Array();//Constructor
    ~Array (); //Destructor
        
};

#endif