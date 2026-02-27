#ifndef tree_hpp
#define tree_hpp
#include "treenode.h"

class tree {
    public:
        treenode* root;
        Array* array;

        //Constructor with parameter
        tree( Array* arr){
            root = nullptr;
            array = arr;}
        
        //Default constructor
        tree(){root = nullptr;}
        
        //Destructor
        ~tree(){
            delete root;
        }

        bool build(const std::string& scode);
        std::string find(double imean, std::string op);
        bool deleteCountry(const std::string& cname);
        bool deleteHelper(treenode* node, const std::string& cname, double imean);
        void limits(const std::string& cond);
        bool remove(std::string ccode);

        bool buildHelper(treenode* node);
};

#endif