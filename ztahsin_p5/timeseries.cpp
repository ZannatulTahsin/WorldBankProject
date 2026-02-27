#include "timeseries2.h"
#include <iostream>
#include <sstream>
#include <fstream>

void TimeSeries::resize(){
    //if the size reaches the capacity double array
    if (size == cap){
        cap *=2;
    //if the size is one-fourth of the capacity halve array
    } else if (size <= cap/4 && cap > 2){
        cap /= 2;
        } else { return;}//safety precaution 
    
    //creating new arrays with the appropriate capacity
    int* new_years = new int[cap];
    double* new_data = new double[cap];
    //copying old data over
    for (int i = 0; i <size; i++){
        new_years[i] = years[i];
        new_data[i] = data[i];
    }
    //deallocating old arrays
    delete[] years;
    delete[] data;
    years = new_years;
    data = new_data;
}

//default constructor
TimeSeries::TimeSeries(): cap(64), years(nullptr), data(nullptr), size(0){
    years = new int[cap];
    data = new double[cap];
}
//default destructor 
TimeSeries::~TimeSeries (){
    delete[] years; years = nullptr;
    delete [] data; data = nullptr;
}
//Access to size variable in main.cpp
int TimeSeries::getSize(){return size;}
std::string TimeSeries::getSeriesName(){return seriesname;}
std::string TimeSeries::getSeriesCode(){return seriescode;}

//Core Functions
//LOAD FUNCTION
    /*
        CITATION: 

        Parts of this function were troubleshooted by a TA. I originally had lines "delete[] years;" to "data = new double[cap]" but deleted them when I was suspicious they were causing errors.
        Later, I ran into an issue where when I loaded two files back to back, the print would print both files instead of just the more recent one. A TA said that I should clear the old arrays just
        in case, so due to that suggestion, I re-included the previously deleted lines. The same TA also pointed out that I should convert member to a double first and THEN filter out the -1s because 
        there may be extra information a string reads that results in -1s being includes. So I originally compared member == "-1", then changed it to std::stod(member) >= 0 as per the TA's suggestion.
    */
    /*
        CITATION: 

        This function was modified by chat.openai.com with the prompt:"how to open a csv file in c++ with only iosteram and sstream"
        Parts of this load function were taken from the preliminary project which I creates with the help of ChatGPT. I did not know any syntax involving fstream and sstream, so I learned those through chat. 
        The following lines/words were from chatGPT: std::ifstream, std::getline(x, y), std::istringstream, std::getline(x, y, ','), file.close. 
    */
    
    /*
        CITATION:

        Due to the fact that we were not taught fstream and stream and dealing with importing files in ECE 150, this was designed in consultation with Emily Hoang and Madeline Wice. We discussed the
        project and came to the conclusion that we should create a variable to incrememnt through each column/cell in the csv file to correctly load data into the arrays. The three of us implemented a
        similar case. 
    */
    bool TimeSeries::loadFromFile(const std::string& series){
    
        std::istringstream file(series);

    if (years){delete[] years;} //deallocates existing data in arrays if any
    if (data) {delete[] data;}
    cap = 2; //re-declares variables just in case they're not set properly
    size = 0;
    years = new int[cap];
    data = new double[cap];
    std::string line; //each line in the file
    int index = 0; //index used in loading data into arrays
    std::string member; //will hold each cell value 
    int column = 0;
        while (std::getline(file, member, ',')){//separates each line into its cells 
            if (column == 0){countryname = member;}
            else if (column == 1){countrycode = member;}
            else if (column == 2){seriesname = member;} // loads seriesname into class 
            else if (column == 3){seriescode = member;} //loads series code into class 
            else if (column > 3){ //Here is where the time series data begins 
                if (std::stod(member) != -1){ //Filtering out -1s/Invalid Data
                    int firstyear = 1960; //Starting point to load years into the years array
                    double datapoint = std::stod(member); //converts cell data from a string to a double
                    if (size == cap){resize();}//checks if capacity needs to be doubled     
                    data[size] = datapoint; //loads cell data into data array
                    years[size++] = firstyear + (column-4); //loads year into years array
                }
            }
        /*if (column < 67){*/column +=1;/*}*/ //increments columns counter 
        }
    return true;
}

    bool TimeSeries::add(int year, double value){
        if (size == 0){ //Takes care of empty array case
            years[0] = year; //Makes the first element in each array the added data
            data[0] = value;
            size +=1; //Increments size
            return true;
        }

        if (value == -1){return false;} //Do not add invalid data

        for (int i = 0; i<size; i++){//iterate through years array
            if (years[i] == year){return false;} //if the year already exists do nothing
        } 
        
        if (size > 1){ //Need to ensure this or else there's a chance we reach indexes that don't exist (less than 0 or greater than the size)
        if (size+1 == cap){resize();}//check for need to resize (e.g. if size was 31 and now is 32)
        for (int i = 0; i<size-1; i++){
            if (years[i] < year && years[i+1] > year){//locate appropriate index for new year
                    int* new_years = new int[cap];//create new arrays
                    double* new_data = new double[cap];
                    for (int j = 0; j<=i; j++){//copy all elements of previous arrays up until the year directly below new year
                        new_years[j] = years[j];
                        new_data[j] = data[j];
                    }
                    new_years[i+1] = year;//insert new year and data in the immediate following index
                    new_data[i+1] = value;
                    for (int k = i+2; k<size+1; k++){//copy the rest of the array 
                        new_years[k] = years[k-1];
                        new_data[k] = data[k-1];
                    }
                delete[] years;//deallocate old arrays
                delete[] data;
                years = new_years;
                data = new_data;
                }
                }
        }
        if (years[0] > year){//Case where the new data is to be added at the beginning of the arrays
        int* new_years = new int[cap];//create new arrays
        double* new_data = new double[cap];
        new_years[0] = year; //Makes the first element the added data 
        new_data[0] = value;
        for (int i = 1; i<size+1; i++){//Shift the rest of the data up 
            new_years[i] = years[i-1];
            new_data[i] = data[i-1];
        }
        delete[] years;//deallocate old arrays
        delete[] data;
        years = new_years;
        data = new_data;
        } else if (years[size-1] < year){//Case where new data is to be added at the end of the array
            years[size] = year; //Adds data to the last index
            data[size] = value;
        }
    size +=1;//increments size          
    if (size >= cap){resize();}//checks if the capacity needs to be doubled
    return true;
}
    bool TimeSeries::update(int time, double value){
        if (value != -1){//Invalid data will not be included 
        for (int i = 0; i<size; i++){//This follows the worst-case runtime being O(N) as explained in design doc
            if (years[i] == time){//Finds matching year and updates the data
                data[i] = value;
                return true;
            }
        }
        } else { return false;}
        return false;
    }
    void TimeSeries::print() const{
        if (size < 1){ //Empty array case 
            std::cout << "failure" << std::endl;
        } else {
        for (int i = 0; i<size; i++){ //Prints out array in ordered pairs (year, data)
            std::cout <<"(" << years[i] <<"," << data[i] << ") ";
            }
        std::cout << std::endl;
    } 
}

//Statistic Functions 
double TimeSeries::mean() const{
    double sum = 0; //Will hold sum of data
    double denom = 0; //Will be the denominator of the formula 
    for (int i = 0; i < size; i++){//Sums all of the data and counts the number of elements to be the denominator 
            sum += data[i];
            denom +=1; 
    }
    if (denom == 0){//Empty array case
        return -1;
    } else{
    double mean = sum/denom;//Mean formula (his mother should have taught him to be nice smh)
    return mean;
    }
}

bool TimeSeries::is_monotonic()const{
    if (size < 3 && size > 0){return true;} else {//If the array size is 1 or 2, it'll always be monotonic because there will only be one increase or decrease (or equal) which means it's all increasing or decreasing
    for (int i= 0; i<size-2; i++){//Iterates through the array to find a breaking case where an increase is followed by a decrease or vice versa
        if ((data[i] < data[i+1] && (data[i+1] > data[i+2]))){
            return false;}
        else if ((data[i] > data[i+1] && (data[i+1] < data[i+2]))){return false;}
    }//This follows the best-case runtime being O(1) as explained in the design doc
    }
    return true;
}

/*
    CITATION:

    This function was modified by chat.openai.com with the prompt: "I don't think I'm getting the correct output for this function. I printed all of the sums and it looks like all sums involving a product are
    incorrect. Why?"
    I originally had all of the declared variables in this function as doubles, but chatGPT suggested I use long longs, which fixed my outputs. 
*/
void TimeSeries::best_fit(double &m, double &b)const{
    if (size == 0){//Empty array case
        std::cout << "failure" << std::endl;
        return;
    }
    //All required sums in the formulas 
    long long sum_xy = 0;
    long long x = 0;
    long long y = 0;
    long long x2 = 0;
    int count = 0;//To act as size variable
        for (int i = 0; i<size; i++){//Sums all required sums by iterating through arrays
        long long product = years[i]*data[i];
        sum_xy += product;
        x += years[i];
        y += data[i];
        x2 += (years[i])*(years[i]);
        count +=1;//Counts number of elements 
    }
    long long slope;
    long long intercept;
    slope = ((count*sum_xy)-(x*y))/(count*x2-(x*x));//Slope formula
    intercept = (y-slope*x)/count;//Intercept formula
    std::cout << "slope is " << slope << " intercept is " << intercept << std::endl;//Output required in project document 
    return;
}

double TimeSeries::setMean(){
    if (mean() < 0){
        meanval = 0;
    } else {
        meanval = mean();
    }
    return meanval;
}