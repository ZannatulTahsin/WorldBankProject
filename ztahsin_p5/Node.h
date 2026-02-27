#ifndef node_hpp
#define node_hpp
#include <string>
#include "timeseries2.h"

class Node {
    public:
    //Data
    TimeSeries* object; //Node data is an object from the timeseries class
    
    //Pointres for doubly linked list 
    Node * next;
    Node * prev;

    Node();//Constructor
    ~Node (); //Destructor
        
};

#endif