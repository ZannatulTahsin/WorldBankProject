#include <iostream>
#include <sstream>
#include <fstream>
#include "timeseries2.h"
#include "country.h"
#include "Node.h"
//#include "timeseries.cpp"

country::country(){
    head = nullptr; tail = nullptr;
}
country::~country(){ //Recursive linked list constructor based on the lecture slides in ECE 250 
    destroy(head);
    head = nullptr; tail = nullptr;
}

Node::Node(): next(nullptr), prev(nullptr){}
Node::~Node (){
    delete object;
    object = nullptr;
    next = nullptr; prev = nullptr;
} 

bool country::loadCountry(const std::string& cline){//Loads time series' of a specified country into linked list 

        bool found = false;
        Node* newNode = new Node();
        newNode->object = new TimeSeries();
        newNode->object->loadFromFile(cline);

        if (head == nullptr){ //empty list case
            head = newNode;
        } else { //non-empty list
            tail->next = newNode;
            newNode->prev = tail;
            }
            tail = newNode;

            this->countryname = newNode ->object->countryname;
            this->countrycode = newNode->object->countrycode;
            
            found = true; 
        return found; //Returns true if the input country name exists in the file, false otherwise 
}

void country::list(){//Lists the country name, code, and time series' of the loaded country 
    std::cout << this->countryname << " " << this->countrycode << " ";
    Node * curr = head; 
    while (curr != nullptr){
        std::cout << curr->object->getSeriesName() << " ";
        curr = curr->next;
    }
    std::cout << std::endl;
}

void country::printSeries(std::string code){//Prints the arrays of the specified time series in ordered pairs 
    Node * curr = head;
    while (curr != nullptr){//Iterates through linked list until matching series code 
        if (curr->object->getSeriesCode() == code){
            curr->object->print();//Calls project 1 print function on TimeSeries object with matching input series code 
            return;
        } else {
        curr = curr->next;
        }
    }
    std::cout << "failure" << std::endl;
};

bool country::addToSeries(std::string code, int year, double value){//Adds a new ordered pair to the TimeSeries object with the specified series code
    Node * curr = head;
    while (curr != nullptr){//Iterates through linked list until it finds matching series code
        if (curr->object->getSeriesCode() == code){
            return(curr->object->add(year, value));//Calls add function from project 1 on that object 
        } else {
        curr = curr->next;
        }
    }
    return false;
}

bool country::updateSeries(std::string code, int year, double value){//Updates an ordered pair in the TimeSeries obejcet with given series code
        Node * curr = head;
    while (curr != nullptr){
        if (curr->object->getSeriesCode() == code){//Iterates through linked list until matching series code found
            return(curr->object->update(year, value));//Calls project 1 update function on object 
        } else {
        curr = curr->next;
        }
    }
    return false;
}

bool country::deleter(std::string code){//Deletes Node with TimeSeries object that has the specified series code
    Node * curr = head;
    while (curr != nullptr){
        if (curr->object->getSeriesCode() == code){
            Node * temp = curr;

            //Head Case
            if (curr == head){    
                head = curr->next;
                if (head != nullptr){head->prev = nullptr;} //Check that there isn't only one node in the list
            }

            //End case
            else if (curr == tail){
                curr->prev->next = nullptr;
                tail = curr->prev; 
            }

            //Middle case
            else{
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;}

            //In all cases, increment current before deleting the node of interest 
            curr = curr->next;
            delete temp;
            return true;

        } else {
            curr = curr->next;
        }
    }
    return false;
}

void country::biggest(){
    Node * curr = head;
    double biggest = 0;//Will hold the current largest mean value while iterating through linked list and save the final largest value 
    std::string ans;//Will hold the series code string of series with highest mean 
    biggest = curr->object->mean();
    while (curr != nullptr){//Iterates through linked list, calling project 1 mean function on each Node
        if (curr->object->mean() > biggest){//Sets biggest to the mean value of the object every time a bigger value is found 
            biggest = curr->object->mean();
            ans = curr->object->getSeriesCode();
        }
        curr = curr->next;
    }
    if (biggest == 0){//No time series with valid data 
        std::cout << "failure" << std::endl;
    } else {
    std::cout << ans << std::endl;}
}

