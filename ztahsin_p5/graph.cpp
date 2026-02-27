#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <stack>
#include "country.h"
#include "Node.h"
#include "array.h"
#include "tree.h"
#include "treenode.h"
#include "edge.h"
#include "graph.h"

bool graph::initialize(Array& arr){
    int counter = 0;
    adjacents.clear();//if a graph exists, clear all edges
    for (int i =0; i<512; i++){
        if (arr.all[i] != nullptr){//iterates through the 512 array and creates a graph node for each country 
            country* gnode = arr.all[i]->countries;
            adjacents[gnode] = {}; //sets up a pair with the key as gnode and an empty submap as the value (no edges) <gnode, {}>
            counter +=1; //used to check during troubleshooting if all required nodes were created (266)
        } 
    }
    //std::cout << counter << " nodes initialized." << std::endl;
    return true;
}

country* graph::nameToPointer(std::string cname){ //takes country name and outputs the appropriate country object pointer, helper for update_edges
    for (const auto& [c, _] : adjacents){
        if (c->countryname == cname){return c;}
    }
    return nullptr;
}

bool graph::update_edge(std::string scode, double thres, std::string rel, tree& tr){
    //build tree with scode
    tr.build(scode);
    //call find on parameters, output is a comma delimited string
    std::string output = tr.find(thres,rel);
    //check if null string, if yes output false, else continue
    if (output == "failure"){return false;}
    std::istringstream out(output);
    //create list to hold list of country names
    std::vector <std::string> cnames; 
    //load function -> go through each "cell" and load country into list 
    std::string cell;
    while (std::getline(out, cell, ',')){
        //add all countries to the list incrementing one index at a time, store value of last index that was input 
        cnames.push_back(cell);
    }
    int edgecreated = 0;
    //Iterate through all indexes of the list and add the country as an adjancent country (neighbour) to all other countries in the list 
    for (int i = 0; i < cnames.size(); i++){
        for (int j = i+1; j < cnames.size(); j++){
            country* c1 = nameToPointer(cnames[i]);
            country* c2 = nameToPointer(cnames[j]);
            if (c1 == nullptr || c2 == nullptr){continue;} //extra check in case country doesn't exist, should never happen

            //if edge doesn't exists between current two countries 
            if (adjacents[c1].find(c2) == adjacents[c1].end()){
                adjacents[c1][c2] = new edge(); //new edge created
                adjacents[c2][c1] = adjacents[c1][c2]; //adding the edge to EACH OTHER
            }
        auto newtup = std::make_tuple(scode, thres, rel); //make new tuple
        bool pass = adjacents[c1][c2]->relations.insert(newtup).second;//adds tuple to set if it doesn't already exist there, saves whether or not it inserted into pass variable
            if (pass){ //if at least one new relationship was added, output success
                edgecreated +=1; 
            }
        }
    }    
    if (edgecreated > 0){return true;}
    else {return false;}
}
void graph::adjacent(std::string ccode){
    for (auto& [c1, submap] : adjacents){//iterate through graph 
        if (c1->countrycode == ccode){//found country of interest
            if (submap.empty()){std::cout << "none" << std::endl; return;} //if no adjacent countries 
            else{
                for (std::map<country*, edge*>::iterator val = submap.begin(); val!=submap.end(); val++){//iterate through map and output all adjacent country  names 
                    std::cout << val->first->countryname << " ";
                }
            }
            std::cout << std::endl;
            return;
        }
    }
    //exited the entire map meaning the country does not exist in map
    std::cout << "failure" << std::endl;
}
bool graph::path(std::string ccode1, std::string ccode2){ //Follows Graph Search Powerpoint Slides from Lectures 
    std::set<country*> visited;
    std::stack<std::vector<country*>> todo;
    std::vector<country*> path;
    for (auto& [c1, submap] : adjacents){
        if (c1->countrycode == ccode1){ //Find "from" country
            todo.push({c1});//push "from" country into queue
            while (!todo.empty()){
                path = todo.top(); //Add the first possible path into the path list 
                todo.pop();
                country* temp = path.back(); //currently pointing to most recent node
                if (temp->countrycode == ccode2){ //If we're at "to" country, we're done 
                    return true; //path exists 
                }
                if (visited.find(temp) == visited.end()){//If we have yet to visit the temp node  
                    visited.insert(temp);//add to the visited set
                    for (auto& [c2, edges] : adjacents[temp]){ //Iterate through submap -> adjacent countries 
                        std::vector<country*> newpath = path; //Create new path of current node + adjacent country for each possible adjacent country 
                        newpath.push_back(c2); 
                        todo.push(newpath); //Add to stack and repeat 
                    }
                }
            }
        }
    }
    return false;
}
void graph::relationships(std::string ccode1, std::string ccode2){
    for (auto& [c1, submap] : adjacents){ //go through map and find c1
        if (c1->countrycode == ccode1){
            for (auto& [c2, edges] : submap){ //go through submap of c1 and find c2
                if (c2->countrycode == ccode2){
                    for (const auto& tup : edges->relations){ //output all tuples
                        std::cout << "(" << std::get<0>(tup) << " " << std::get<1>(tup) << " " << std::get<2>(tup) << ") ";
                    }
                    std::cout << std::endl;
                    return;
                }
            }
        }
    }
    //exited for loops so no match was found between two input countries 
    std::cout << "none" << std::endl;
    return;
}