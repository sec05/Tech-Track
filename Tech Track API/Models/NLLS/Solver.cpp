#include "Solver.h"

#include <armadillo>

#include <iostream>

Solver::Solver(const arma::dmat& data) {
    data_ = data;
}
Solver::~Solver() {
}

void Solver::ComputeJacobian(const arma::dvec& coeff) {
    int N = data_.n_rows;
    int M = coeff.n_elem;
    J_ = arma::zeros<arma::dmat>(N, M);
    for (int i = 0; i < N; ++i) {
        double x = data_(i, 0);
        J_(i, 0) = sin(coeff(1) * x);
        J_(i, 1) = coeff(0) * x * cos(coeff(1) * x);
        J_(i, 2) = cos(coeff(3) * x);
        J_(i, 3) = -coeff(2) * x * sin(coeff(3) * x);
    }
}

void Solver::ComputeResiduals(const arma::dvec& coeff) {
    int N = data_.n_rows;
    residuals_ = arma::zeros<arma::dvec>(N);
    for (int i = 0; i < N; ++i) {
        double x = data_(i, 0);
        residuals_(i) = data_(i, 1) - (coeff(0) * sin(coeff(1) * x) + coeff(2) * cos(coeff(3) * x));
    }
}
arma::vec Solver::Solve(int max_iterations, double tolerance) {
    int N = data_.n_rows;
    int M = 4; // Number of coefficients
    coeff_ = arma::ones<arma::dvec>(M);
    coeff_(0) = 1.0; // Initial guess for a
    coeff_(1) = 2.0; // Initial guess for b
    coeff_(2) = 3.0; // Initial guess for c
    coeff_(3) = 4.0; // Initial guess for d
    // Initialize residuals and Jacobian
    residuals_ = arma::zeros<arma::dvec>(N);
    J_ = arma::zeros<arma::dmat>(N, M);

    for (int iter = 0; iter < max_iterations; ++iter) {
        ComputeResiduals(coeff_);
        ComputeJacobian(coeff_);

        arma::dvec delta = arma::solve(J_.t() * J_, -J_.t() * residuals_);
        coeff_ += delta;
        printf("Iteration %d: Coefficients: %f, %f, %f, %f\n", iter, coeff_(0), coeff_(1), coeff_(2), coeff_(3));
        printf("Residuals: %f\n", arma::norm(residuals_));
        printf("Delta: %f\n", arma::norm(delta));
        // Check for convergence

        if (arma::norm(delta) < tolerance) {
            break;
        }
    }
    printf("Final Coefficients: %f, %f, %f, %f\n", coeff_(0), coeff_(1), coeff_(2), coeff_(3));
    printf("Final Residuals: %f\n", arma::norm(residuals_));
    return coeff_;
}
