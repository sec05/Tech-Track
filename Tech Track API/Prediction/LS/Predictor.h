// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_MODELS_NLLS_PREDICTOR_H_
#define TECH_TRACK_API_MODELS_NLLS_PREDICTOR_H_

#include "Solver.h"
#include "DummyDataGenerator.h"

class Predictor{
 public:
    Predictor(int number_points, int lower_bound, int upper_bound);
    ~Predictor();

    double* Predict(const double* times, const int num_times);

 private:
    DummyDataGenerator dummy_generator_;
    Solver solver_;
};

#endif  // TECH_TRACK_API_MODELS_NLLS_PREDICTOR_H_
