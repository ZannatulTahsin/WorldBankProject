#ifndef countries_hpp
#define countries_hpp
#include "Node.h"
#include "timeseries2.h"
#include <string>

class country { //This is my country linked list - will hold the nodes of each time series 
    private:

        //Helper function for my destructor for recursive implementation 
        void destroy(Node * n){
            if (n != nullptr){//Goes to the end of the linked list and starts deleting backwards 
                destroy(n->next);
                delete n;
            }
        };

    public:
        
        Node * head;
        Node * tail;

        //Country name and code
        std::string countryname;
        std::string countrycode;
        //Constructor and Destructor 
        country();
        ~country();
        
        //Core functions for project 2
        bool loadCountry(const std::string& cname);
        void list();
        void printSeries(std::string code);
        bool addToSeries(std::string code, int year, double value);
        bool updateSeries(std::string code, int year, double value);
        bool deleter(std::string code);
        
        
        //Statistical Function(s) for project 2
        void biggest();


       
};

#endif