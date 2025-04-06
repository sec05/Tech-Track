// Copyright 2025 Spencer Evans-Cole

#include "Predictor.h"

#include <armadillo>
#include <cmath>
#include "Solver.h"
#include "DummyDataGenerator.h"

Predictor::Predictor(int number_points, int lower_bound, int upper_bound)
    : dummy_generator_(number_points, lower_bound, upper_bound),
      solver_(dummy_generator_.GenerateSinusoidalData(1, 2, 3, 4, 1, 1)) {
}

Predictor::~Predictor() {
}

double* Predictor::Predict(const double* times, const int num_times) {
    arma::cx_mat coeff = solver_.Solve();  // DFT coefficients
    int N = coeff.n_rows;  // Number of original time samples
    double* results = new double[num_times];

    for (int i = 0; i < num_times; ++i) {
        std::complex<double> sum = 0.0;
        for (int k = 0; k < N; ++k) {
            double exponent = -2.0 * M_PI * k * times[i] / N;
            sum += coeff(k, 0) * std::exp(std::complex<double>(0, exponent));
        }
        results[i] = sum.real();  // Take real part if the original data was real
    }

    return results;
}
