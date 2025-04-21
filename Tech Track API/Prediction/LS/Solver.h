// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_PREDICTION_LS_SOLVER_H_
#define TECH_TRACK_API_PREDICTION_LS_SOLVER_H_
#include <armadillo>
class Solver{
 public:
        explicit Solver(const arma::dmat& data);
        ~Solver();
        arma::cx_mat Solve();

 private:
        arma::dmat data_;
        arma::cx_mat coeff_;
        arma::cx_mat FourierSolve();
};
#endif  // TECH_TRACK_API_PREDICTION_LS_SOLVER_H_
