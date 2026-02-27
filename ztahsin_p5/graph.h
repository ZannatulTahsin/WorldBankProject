#ifndef graph_hpp
#define graph_hpp
#include <map>
#include "country.h"
#include "edge.h"
#include "array.h"
#include "tree.h"

class graph {
    public:
        std::map <country*, std::map <country*, edge*>> adjacents; 
        graph() {}
        /*
        CITATION: 

        This following function was modified by chat.openai.com with the prompt:
        "success
        free(): double free detected in tcache 2
        Aborted (core dumped)

        this was the output of the update_edges function" 
        Lines written by chatGPT: "if(free.find(edge) == free.end()){", "free.insert(edge)"

        */
        ~graph(){
            std::set<edge*> free;
            for (auto& [country, neighbor] : adjacents){//iterates through all members of the set and deletes them 
                for (auto& [neighbor, edge] : neighbor){
                    if(free.find(edge) == free.end()){ //The destructor was trying to delete edges that had already been freed in the destruction of another edge
                    delete edge;
                    free.insert(edge); //Now we check of the edge exists before trying to free it
                    }
                }
            }
        }
        bool initialize(Array& arr);
        country* nameToPointer(std::string cname);
        bool update_edge(std::string scode, double thres, std::string rel, tree& tr);
        void adjacent(std::string ccode);
        bool path(std::string ccode1, std::string ccode2);
        void relationships(std::string ccode1, std::string ccode2);

};

#endif