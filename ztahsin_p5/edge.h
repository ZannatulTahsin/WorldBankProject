#ifndef edge_hpp
#define edge_hpp
#include <string>
#include <set>
#include <tuple>

class edge {
    //Set of all of the tuples between two countries 
    public:
        std::set<std::tuple<std::string, double, std::string>> relations; 
        edge () {}
        ~edge(){}
       
};

#endif