#include "timeseries2.h"
#include "Node.h"
#include "country.h"
#include "array.h"
#include "tree.h"
#include "treenode.h"
#include "edge.h"
#include "graph.h"
#include <iostream>
#include <string>

int main() {
    Array arr;
    tree tr(&arr);
    graph grr; 
    std::string command; //Hold user input command

    while (std::cin >> command){//This whole while loop follows case by case every output of each function according to the Project Document
        if (command == "LOAD"){
            std::string filename;
            std::cin >> filename;
            if (arr.loadAllCountries(filename)){
                std::cout<<"success"<<std::endl;
            } 
        } else if (command == "LIST"){
            std::string cname;
            std::cin >> cname;
            arr.listCountry(cname);
        } else if (command == "RANGE"){
            std::string scode;
            std::cin >> scode;
            arr.range(scode);
        } else if (command == "BUILD"){
            std::string scode;
            std::cin >> scode;
            //tree tr(&arr);
            if (tr.build(scode)){
                std::cout << "success"<<std::endl;
            }
        }else if (command == "FIND"){
            double mean;
            std::string operation;
            std::cin >> mean;
            std::cin >> operation;
            std::cout << tr.find(mean, operation) << std::endl;
        }else if (command == "DELETE"){
            std::string cname;
            std::cin >> cname;
            if (tr.deleteCountry(cname)){
                std::cout << "success"<<std::endl;
            } else {
                std::cout <<"failure" << std::endl;
            }
        }else if (command == "LIMITS"){
            std::string cond;
            std::cin >> cond;
            tr.limits(cond);
        }else if (command == "LOOKUP"){
            std::string ccode;
            std::cin >> ccode;
            arr.lookup(ccode);
        } else if (command == "REMOVE"){
            std::string ccode;
            std::cin >> ccode;
            if (tr.remove(ccode)){
                std::cout << "success"<<std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (command == "INSERT"){
            std::string ccode;
            std::string filename;
            std::cin >> ccode;
            std::cin >> filename;
            if (arr.insert(ccode, filename)){
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (command == "INITIALIZE"){
            if (grr.initialize(arr)){
                std::cout << "success" << std::endl;
            }
        } else if (command == "UPDATE_EDGES"){
            std::string scode; std::string rel; double thres;
            std::cin >> scode;
            std::cin >> thres;
            std::cin >> rel;
            if (grr.update_edge(scode, thres, rel, tr)){
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure"<<std::endl;
            }
        } else if (command == "ADJACENT"){
            std::string ccode;
            std::cin >> ccode;
            grr.adjacent(ccode);
        } else if (command == "RELATIONSHIPS"){
            std::string ccode1;
            std::string ccode2;
            std::cin >> ccode1;
            std::cin >> ccode2;
            grr.relationships(ccode1, ccode2);
        } else if (command == "PATH"){
            std::string ccode1;
            std::string ccode2;
            std::cin >> ccode1;
            std::cin >> ccode2;
            if (grr.path(ccode1, ccode2)){
                std::cout << "true" << std::endl;
            } else {
                std::cout << "false" << std::endl;
            }
        }else if (command == "EXIT"){
            break;
        }
    }
}