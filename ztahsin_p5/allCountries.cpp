#include <iostream>
#include <sstream>
#include <fstream>
#include "country.h"
#include "Node.h"
#include "array.h"
#include "tree.h"
#include "treenode.h"
#include <cmath>

    //Constructor
    Array::Array(){
        for (int i = 0; i < 512; i++){
            all[i] = nullptr; 
        } 
    }
    //Destructor
    Array::~Array (){
        for (int i = 0; i < 512; i++){
                delete all[i];
        }
    }

bool Array::loadAllCountries(const std::string& filename){
    //open file
    std::ifstream file(filename); 
    if (!file.is_open()){ //Checks file 
    return false;
    }

    //Changes start here 
    std::string currentcountry = ""; //holds current country code
    country* currObj = nullptr; 
    hash* currHash = nullptr;
    int index = 0;

    //read first line and second column (country code)
    std::string line, rec;
    while(std::getline(file, line)){
        std::istringstream countrypass(line); //first line now being read
        int column = 0;
        while (std::getline(countrypass, rec, ',')){
            if (column == 1 && rec != currentcountry){//if the second box is NOT nothing
                //set current country to current box's code 
                currentcountry = rec;
                currObj = new country();
                //hash current country code
                int h1 = hasher(currentcountry); //first hash result
                int h2 = secondhash(currentcountry);//second hash result
                //check if you can insert into current index
                bool canInsert = false;
                int i = 0;
                while (canInsert == false){
                    index = (h1 + (i*h2))%512;
                    if (all[index] == nullptr){
                        currHash = new hash(); //only create new hash object if we're to insert inside a never occupied index 
                        //insert hash object here
                        all[index] = currHash;
                        //set hash objects countries object to the current country's linked list
                        all[index]->countries = currObj;
                        all[index]->isOcc = true;
                        canInsert = true;
                        all[index]->ind = index;
                    } else if (all[index]->countries == nullptr && all[index]->isOcc == true){//previously occupied 
                        currHash = all[index];
                        all[index]->countries = currObj;
                        all[index]->ind = index;
                        canInsert = true;
                    }else { //currently there's already an object in that index
                        i++;
                    }
                }
            }
            column ++;
        }
                
            //pass the current line as a string stream to loadCountry
            if(currHash && currHash->countries){currHash->countries->loadCountry(line);}
    }
    /*for (int i = 0; i < 512; i++){
        if (all[i] != nullptr){
            std::cout << all[i]->countries->countrycode << " " << all[i]->ind << std::endl;
        }
    }*/
    file.close();
    return true; 
}

int Array::hasher(std::string ccode){
    int cone = ccode[0]-65;
    int ctwo = ccode[1]-65;
    int cthree = ccode[2]-65;
    int w = (cone*676) + (ctwo * 26) + cthree;
    int index = w%512;
    return index; 
}

int Array::secondhash(std::string ccode){
    int cone = int(ccode[0])-65;
    int ctwo = int(ccode[1])-65;
    int cthree = int(ccode[2])-65;
    int w = (cone*676) + (ctwo * 26) + cthree;
    int test = (w/512)%512;
    int index = 0;
    if (test%2 == 1){
        index = test;
    } else {
        index = test + 1;
    }
    return index;
}

void Array::listCountry(std::string cname){//Lists the country name, code, and time series' of the loaded country 
    for (int i = 0; i < 512; i++){
        if (all[i] != nullptr && all[i]->countries != nullptr && all[i]->countries->countryname == cname){
            all[i]->countries->list();
        }
    }
}

void Array::range(std::string scode){
    double max = -1e9;
    double min = 1e9;
    //for loop to iterate through all countries
    for (int i = 0; i < 512; i++){
        if (all[i] == nullptr || all[i]->countries == nullptr){continue;}
        Node* point = all[i]->countries->head;
        while(point != nullptr){
            if (point->object->getSeriesCode() == scode){//if the series code of a country equals scode then take the mean
                if (point->object->mean() > max){//if that mean is bigger than current max, set current max to that mean
                    max = point->object->mean();
                }
                if (point->object->mean() < min && point->object->mean() != -1){//if that mean is smaller than current min, set current min to that mean
                    min = point->object->mean();
                }
            }
            point = point->next;
        }
    }
    //Print output
    std::cout << min << " "<< max << std::endl;
}

bool tree::build(const std::string& scode){
    //1. Delete any exisiting tree
    if(root){delete root;}
    root = new treenode();

    int count = 0; double tempmax = -1e9; double tempmin = 1e9;
    int ind = 0;

    //2. Store all countries in root, count number of countries
    for (int i = 0; i<512; i++){
        if (array->all[i] == nullptr){continue;}//checks that index isn't null
        //if all[i] has series with matching series code, add it to the root's array
        Node* point = array->all[i]->countries->head;
        while (point != nullptr){
            if (point->object->getSeriesCode() == scode){
                count +=1;
                root->countries[ind++] = point->object;
                //3. get min and max values
                if (point->object->mean() < 0){//sets mean of all invalid data to 0
                    point->object->meanval = 0;
                } else {
                    point->object->meanval = point->object->mean();
                }
                if (point->object->meanval > tempmax){tempmax = point->object->meanval;}
                if (point->object->meanval < tempmin){tempmin = point->object->meanval;}
            }
            point = point->next;
        }
    }
    /*for (int i = 0; i < 512; i++){
        if (root->countries[i] != nullptr){
            std::cout << root->countries[i]->countrycode << " ";
        }
    }*/
    if (count == 0){
        delete root;
        root = nullptr;
        return true;
    }
    root->min = tempmin; root->max = tempmax; root->midpoint = (tempmin + tempmax)/2; root->countrycount = count;
    /*for (int i = 0; i<512; i++){
        if (root->countries[i] != nullptr){
            std::cout << root->countries[i]->countrycode <<  " ";
        }
    }*/
    //std::cout << std::endl;
    return buildHelper(root);
}

bool tree::buildHelper(treenode* node){
    //4. Basecases:
    //If there is only one country in the node, stop:
    if (node == nullptr){return true;}
    if (node->countrycount == 1){
        return true;} 
    //If all the countries in the node have the same mean
        if ((node->max - node->min) <= 1e-3 || node->max == node->min){
        return true;
    }
    
  //BASE CASES DONE STARTING ACTUAL ALGORITHM NOW  
    node->midpoint = (node->min + node->max)/2.0;
//4.5 Check whether or not child nodes need to be created
bool makeleft = false; bool makeright = false;
for (int i = 0; i < node->countrycount; i++){
    if (node->countries[i] != nullptr){
    if (node->countries[i]->setMean() < node->midpoint){makeleft = true;}
    if (node->countries[i]->setMean() >= node->midpoint){makeright = true;}
    if (makeleft && makeright) {break;}
    }
}
if (!makeright && !makeleft){return true;}//no countries

//5. Making children nodes (left)
if (makeleft){
    treenode* lchild = new treenode();
    node->left = lchild;
    lchild -> min = node->min;
    lchild->max = node->midpoint;
    lchild->midpoint = (lchild->min + lchild->max)/2.0;

    //Loading array
    for (int i = 0; i < node->countrycount;i++){
        if (node->countries[i] != nullptr){
        if (node->countries[i]->setMean() < node->midpoint && node->countries[i]->setMean() >= lchild->min){
            lchild->countrycount ++;
            lchild->countries[lchild->countrycount-1] = node->countries[i];
        }
    }
    }
}

//6. Making children nodes (right)
if (makeright){
    treenode* rchild = new treenode();
    node->right = rchild;
    rchild->max = node->max;
    rchild->min = node->midpoint;
    rchild->midpoint = (rchild->min + rchild->max)/2.0;

    //Loading array
    for (int i = 0; i < node->countrycount; i++){
        if (node->countries[i] != nullptr){
        if (node->countries[i]->setMean() >= node->midpoint && node->countries[i]->setMean() <= rchild->max){
            rchild->countrycount ++;
            rchild->countries[rchild->countrycount-1] = node->countries[i];
        }
    }
    }
}

//7. Recurse
if (node->left != nullptr && node->left->countrycount > 0){
    buildHelper(node->left);
}
if (node->right != nullptr && node->right->countrycount > 0){
    buildHelper(node->right);
}
    return true;
}

std::string tree::find(double imean, std::string op){
    //changing to output comma delimited string 
    //if no binary tree exists, output "failure"
    if (root==nullptr){return "failure";}
    std::ostringstream output;
    bool firstcountry = true; //keeping track of first country

    for (int i = 0; i < 512; i++){
        bool crit = false;
    //3 cases for greater, less than, or equal to
    //1. Greater
    //If op is "greater", then iterate through the array in the root node and output all countries with means greater than imean
if (root->countries[i] == nullptr){continue;}
if (root->countries[i] != nullptr){
    //std::cout << "entered for loop" << std::endl;
    if (op == "greater"){
            if (root->countries[i]->setMean() > imean){
                crit = true;
            }
    }
    //2. Less than 
    //If op is "less than", iterate through array in the root node and output all countries with means less than imean
    if (op == "less"){
            if (root->countries[i]->setMean() < imean){
                crit = true;
            }
    }
    //3. If the string is "equal", iterate through root node array and output all countries with equal imean
    if (op == "equal"){
            if (fabs(root->countries[i]->setMean()-imean) < 0.001){
                crit = true;
            }
        }
        if (crit){
            if (!firstcountry){
                output<<","; //if we're not at the first country, add a coma
            }
            output << root->countries[i]->countryname; //add the country name
            firstcountry = false; 
        }
    }
    }
    return output.str();
}
bool tree::deleteCountry(const std::string& cname){
    if (root == nullptr){
        return false;
    } //if no tree exists return false
    bool pass = false;
    double m;
    for (int i = 0; i<512; i++){
        if (root->countries[i] != nullptr && root->countries[i]->countryname == cname){//country found
            pass = true;
            m = root->countries[i]->setMean();
            for (int j = i; j < root->countrycount+1;j++){//delete index and move above elements down
                root->countries[j] = root->countries[j+1];
            }
            root->countries[root->countrycount-1] = nullptr;
            root->countrycount --;
            break;
            }
        }
        if (pass == false){return false;}//Country doesn't exist in tree
        /*for (int i = 0; i<512; i++){
            if (root->countries[i] != nullptr){
                std::cout << root->countries[i]->countrycode <<  " ";
            }
        } */
        //std::cout << std::endl;
    bool deleted = deleteHelper(root, cname, m);
    if (root->countrycount == 0 && root->left == nullptr && root->right == nullptr){
        delete root;
        root = nullptr;
        return true;
    }
    return deleted;
}

/*
    CITATION: 

    Because both of us were not passing test cases 8, 9, and 11, Madeline Wice and I brainstormed possible issues and fixes for our Delete Function together. She suggested that the "groups of code"
    (e.g. lines 276 to 287 (we called it the right leaf finder section) and lines 291 and 297(we called it the remove from index section)) be in this order.
*/

bool tree::deleteHelper(treenode* node, const std::string& cname, double imean){
    if (node == nullptr){
        return false;
    }
    //Basecases: Leaf Nodes
    //Left is a leaf node:
    if (node->left != nullptr){
        if (node->left->min <= imean && node->left->max > imean){//if cname goes left
            if (node->left->countrycount == 1){//if leaf node
                delete node->left;//Remove
                node->left = nullptr; 
                return true;
            } else {
                return deleteHelper(node->left, cname, imean);
            }
        }
    }
     //Right is a leaf node:
     if (node->right != nullptr){
        if (node->right->min <= imean && node->right->max >= imean){//if cname goes right
            if (node->right->countrycount == 1){//if leaf node
                delete node->right;//Remove
                node->right = nullptr; 
                return true;
            } else {
                return deleteHelper(node->right, cname, imean);
            }
        }
    }
    
//Since other functions rely on for loops that stop when at the first nullptr of the node, we must shift the nodes down to fill the now empty spot
//aka deleting the country from the array
    for (int i = 0; i<node->countrycount; i++){
        if (node->countries[i] != nullptr && node->countries[i]->countryname == cname){
            for (int j = i; j < node->countrycount+1;j++){
                node->countries[j] = node->countries[j+1];
            }
            node->countries[node->countrycount-1] = nullptr;
            node->countrycount --;
        //If we deleted the only country in the list from a leaf node
        if (node->left == nullptr && node->right == nullptr && node->countrycount == 0){
            delete node;
            return true;
        }
        //Delete country from left nodes recursively 
        if (node->countrycount > 0 && node->left != nullptr && node->midpoint > imean){
            return deleteHelper(node->left, cname, imean);
        }
        //Delete country from right nodes recursivly 
        if (node->countrycount > 0 && node->right != nullptr && node->midpoint <= imean){
            return deleteHelper(node->right, cname, imean);
        }
        return true;      
    }
}
//If nothing above returned (hypothetically should never get to this) 
     return false;
}

void tree::limits(const std::string& cond){
    if (root == nullptr){
        std::cout << "failure" << std::endl;
        return;
    }
    //if cond = lowest -> iterate left through tree until you reach node with no left children, print out all countires
    if (cond == "lowest"){
        if (!(root)){std::cout << "failure" << std::endl; return;}
        treenode* curr = root;
        while (curr->left != nullptr){
            curr = curr->left;
        }
        for (int i = 0; i < curr->countrycount; i++){
            if (curr->countries[i] != nullptr){
                std::cout <<curr->countries[i]->countryname << " ";
            }
        }
    }
    //if cont = highest -> iterate right through tree until you reach node with no right children, print out all countries
    if (cond == "highest"){
        if (!(root)){std::cout << "failure" << std::endl; return;}
        treenode* curr = root;
        while (curr->right != nullptr){
            curr = curr->right;
        }
        for (int i = 0; i < curr->countrycount; i++){
            if (curr->countries[i] != nullptr){
                std::cout <<curr->countries[i]->countryname << " ";
            }
        }
    }
    std::cout << std::endl;
}

void Array::lookup(std::string ccode){
    int h1 = hasher(ccode);
    int h2 = secondhash(ccode);
    int i = 0;
    int index;
    bool found = false;
    index = h1%512;
    while (index < 512 && all[index] != nullptr){
            if (all[index]->isOcc == true){
                if (all[index]->countries == nullptr){
                    //std::cout << "country was removed here" << std::endl;
                    i++;
                    index = (h1+(i*h2))%512;
                } else if (all[index]->countries->countrycode == ccode){
                    found = true;
                    break;
                } else {
                    i++;
                    index = (h1+(i*h2))%512;
                    //std::cout << "next index is " << index << std::endl;
                }
        }
    }
    if (!found){std::cout << "failure" << std::endl;}
    int searches = i+1;
    if (found){std::cout << "index " << index << " searches " << searches << std::endl;}
}

bool tree::remove(std::string ccode){
    //find the index of the country code
    int h1 = array->hasher(ccode);
    int h2 = array->secondhash(ccode);
    int i = 0;
    int index;
    bool found = false;
    index = h1%512;
    while (index < 512 && array->all[index] != nullptr){
            if (array->all[index]->isOcc == true){
                if (array->all[index]->countries == nullptr){//if the index was once occupied 
                    i++; 
                    index = (h1+(i*h2))%512;
                } else if (array->all[index]->countries->countrycode == ccode){
                //std::cout << "found at " << index << std::endl;
                found = true;
                //check if a binary tree exists
                if (root){
                    //std::cout << "tree exists, deleting "<<array->all[index]->countries->countryname << std::endl;
                    //if yes, call delete on that country, otherwise do nothing
                    if (!deleteCountry(array->all[index]->countries->countryname)){std::cout << "failed to delete from tree" << std::endl;}
                }
                //if the country exists (country object) then remove it
                delete array->all[index]->countries;
                array->all[index]->countries = nullptr;
                //DO NOT CHANGE ISOCC
                //if (array->all[index]->isOcc == true){std::cout << "is still occupied " << std::endl;}
                break;
            } else {
                i++;
                index = (h1+(i*h2))%512;
            }
        }
            
    }
    //if the country doesn't exist in the array, output failure
    if (!found){return false;}
    return true;
}

bool Array::insert(std::string ccode, std::string filename){ 
            //open csv and check 
            std::ifstream file(filename); 
            if (!file.is_open()){ //Checks file 
            //std::cout << "file is not opening" << std::endl;
            return false;
            }
            //check if a) an array exists b)if it does, is this country already in it
            bool found = false;
            int h1 = hasher(ccode);
            int h2 = secondhash(ccode);
            int index = 0;
            int i = 0;
            index = h1%512;
            while (index < 512 && all[index] != nullptr){
                    if (all[index]->isOcc == true){ //There is or was once a country occupying this index 
                        if (all[index]->countries == nullptr){ //currently there is none (imagine we delete andorra and are trying to insert canada)
                            i++;
                            index = (h1+(i*h2))%512;
                        } else if (all[index]->countries->countrycode == ccode){//there is a country here and it's the one we're trying to insert
                            //std::cout << "country exists in array " << std::endl;
                            found = true;
                            return false;
                        } else {
                            i++;
                            index = (h1+(i*h2))%512;
                        }
                }
            }
            //if we exited the loop, the country was not found
            if (!found){
            //std::cout << "country does not exist in array, carry on with insertion" << std::endl;
            
            std::string curr = "";
            //find all lines with ccode
            std::string piece, box;
            while(std::getline(file, piece)){ //reading lines
            std::istringstream passString(piece);
                bool canInsert = false;
                int column = 0;
                while (std::getline(passString, box, ',')){ //reading columns
                    if (column == 1 && box == ccode){//found a line with matching country code
                        //std::cout << box << std::endl;
                        //if it's the first line with said country code, create new country and hash object
                        if (box != curr){
                        curr = box;
                        //std::cout <<"found first line of country " << box << std::endl;
                        int i = 0;
                        while (canInsert == false){
                            index = (h1 + (i*h2))%512;
                            //std::cout <<"trying at index " << index << std::endl;
                            if (all[index] == nullptr || all[index]->countries == nullptr){ //find the first empty index or first previously occupied space
                                if (all[index] == nullptr){
                                    //std::cout << "found first empty spot" << std::endl;
                                    //if it's an empty index create new hash object to occupy index 
                                    hash* currHash = new hash();
                                    country* currObj = new country();
                                    all[index] = currHash;
                                    all[index]->countries = currObj;
                                    all[index]->isOcc = true;
                                    all[index]->ind = index;
                                    canInsert = true; //stop loop as object has been inserted
                                    //std::cout << "inserted new hash at " << index << std::endl;
                                } else if (all[index]->countries == nullptr && all[index]->isOcc == true){ //if it's previously occupied, only create new country object
                                    //if a hash object already exists
                                    //std::cout << "found first previously occupied spot"<<std::endl;
                                    country* currObj = new country();
                                    all[index]->countries = currObj;
                                    canInsert = true;
                                    //std::cout << "inserted at " << index << std::endl;
                                }
                        } else {i++;} //currently a valid object at index
                    }
                }
                all[index]->countries->loadCountry(piece);
                //std::cout << "loading new timeseries of " << box << std::endl;
                } 
                column++; 
            }
            //std::cout <<"calling load country" << std::endl;
        }
    }
    /*for (int i = 0; i < 512; i++){
        if (all[i] != nullptr){
            std::cout << all[i]->countries->countrycode << " " << all[i]->ind << " ";
        }
    }*/   
            file.close();
            return true; //Returns true if the input country name exists in the file, false otherwise 
}