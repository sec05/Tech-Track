// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_PREDICTION_LS_DUMMYDATAGENERATOR_H_
#define TECH_TRACK_API_PREDICTION_LS_DUMMYDATAGENERATOR_H_

#include <armadillo>

class DummyDataGenerator {
 public:
        DummyDataGenerator(int number_points, int lower_bound, int upper_bound);
        ~DummyDataGenerator();
        arma::mat GenerateSinusoidalData(
            double a, double b, double c,
            double d, double mean, double stddev);
        arma::mat GenerateSinusoidalDataMultipleCompanies(
            double a, double b, double c, double d,
            double mean, double stddev, int num_companies);
        arma::dvec GetTimes() const { return data_.col(0); }
 private:
        int numberPoints_;
        arma::mat data_;
};
#endif  // TECH_TRACK_API_PREDICTION_LS_DUMMYDATAGENERATOR_H_
