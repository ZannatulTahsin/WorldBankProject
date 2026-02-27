#ifndef timeseries2_hpp
#define timeseries2_hpp
#include <string>
//#include "country.h"

class TimeSeries {
    private:
        //series name and code from the time series data 
        std::string seriesname;
        std::string seriescode;
        
        //Dynamically Allocated Arrays 
        int* years;
        double* data;
        //Array variables (size and capacity)
        int size;
        int cap;
        
        //Function to resize arrays
        void resize(); 

    public:
    
        //country name and country code
        std::string countryname;
        std::string countrycode;
        double meanval;
        
        TimeSeries(); // Default Constructor
        ~TimeSeries (); //Destructor

        int getSize(); //Need to access size for the MONOTONIC command
        std::string getSeriesName();
        std::string getSeriesCode();
        
        //Core Functions
        bool loadFromFile(const std::string& series);
        bool add(int year, double value);
        bool update(int year, double value);
        void print() const;
        
        //Statistic Functions 
        double mean() const;
        bool is_monotonic() const;
        void best_fit(double &m, double &b) const;

        double setMean();


        
};

#endif