// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_MODELS_NLLS_LSPREDICTOR_H_
#define TECH_TRACK_API_MODELS_NLLS_LSPREDICTOR_H_

#include "Solver.h"
#include "DummyDataGenerator.h"

class LSPredictor{
 public:
    LSPredictor(arma::dvec data);
    ~LSPredictor();

    arma::dvec Predict();

 private:
    Solver solver_;
   arma::dvec data_;
};

#endif  // TECH_TRACK_API_MODELS_NLLS_PREDICTOR_H_
