// Copyright 2025 Spencer Evans-Cole

#include "LSPredictor.h"

#include <armadillo>
#include <cmath>
#include "Solver.h"
#include "DummyDataGenerator.h"

LSPredictor::LSPredictor(arma::dvec data)
    :data_(data), solver_(data) {
}

LSPredictor::~LSPredictor() {
}

arma::dvec LSPredictor::Predict() {
    int num_times = 5;
    arma::cx_mat coeff = solver_.Solve();  // DFT coefficients
    int N = coeff.n_rows;  // Number of original time samples
    arma::dvec results(num_times);
    for (int i = 0; i < num_times; ++i) {
        std::complex<double> sum = 0.0;
        for (int k = 0; k < N; ++k) {
            double exponent = -2.0 * M_PI * k * (N+i+1) / N;
            sum += coeff(k, 0) * std::exp(std::complex<double>(0, exponent));
        }
        results[i] = sum.real();  // Take real part if the original data was real
    }

    return results;
}
