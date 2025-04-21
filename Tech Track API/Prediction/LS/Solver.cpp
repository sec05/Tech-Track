// Copyright 2025 Spencer Evans-Cole
#include "Solver.h"

#include <armadillo>

#include <iostream>

Solver::Solver(const arma::dmat& data) {
    data_ = data;
}
Solver::~Solver() {
}

arma::cx_mat Solver::Solve() {
    // Perform a Fourier transform on the data
    coeff_ = FourierSolve();
    return coeff_;
}

arma::cx_mat Solver::FourierSolve() {
    arma::cx_mat coeff = arma::fft(data_, data_.n_elem);  // Perform FFT along the first dimension
    return coeff;
}

