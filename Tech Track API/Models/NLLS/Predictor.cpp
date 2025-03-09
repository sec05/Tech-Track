// Copyright 2025 Spencer Evans-Cole

#include "Predictor.h"

#include <armadillo>
#include <cmath>
#include "Solver.h"
#include "DummyDataGenerator.h"

Predictor::Predictor(int number_points, int lower_bound, int upper_bound)
    : dummy_generator_(number_points, lower_bound, upper_bound),
      solver_(dummy_generator_.GenerateSinusoidalData(1,2,3,4,1,1)) {
}

Predictor::~Predictor(){

}

double* Predictor::Predict(const double* times, const int num_times){
    arma::vec coeff =  solver_.Solve(10000, 1e-8);
    printf("Coefficients: %f, %f, %f, %f\n", coeff(0), coeff(1), coeff(2), coeff(3));
    double results[num_times];
    for(int i = 0; i < num_times; i++){
        results[i] = coeff(0) * sin(coeff(1)*times[i]) + coeff(2) * cos(coeff(3)* times[i]);
        printf("Predicted value at time %f: %f\n", times[i], results[i]);
    }
    return results;
}