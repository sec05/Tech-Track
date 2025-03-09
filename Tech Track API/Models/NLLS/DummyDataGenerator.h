// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_NLSS_DUMMYDATAGENERATOR_H_
#define TECH_TRACK_API_NLSS_DUMMYDATAGENERATOR_H_

#include <armadillo>


class DummyDataGenerator {
    public: 
        DummyDataGenerator(int number_points, int lower_bound, int upper_bound);
        ~DummyDataGenerator();
        arma::mat GenerateSinusoidalData(double a, double b, double c, double d, double mean, double stddev);
    
        private:
        int numberPoints_;
        arma::mat data_;      
};
#endif  // TECH_TRACK_API_NLSS_DUMMYDATAGENERATOR_H_