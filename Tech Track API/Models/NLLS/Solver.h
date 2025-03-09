// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_NLSS_SOLVER_H_
#define TECH_TRACK_API_NLSS_SOLVER_H_
#include <armadillo>
class Solver{
    public: 
        Solver(const arma::dmat& data);
        ~Solver();
        arma::vec Solve(int max_iterations, double tolerance);

        private:
        arma::dmat data_;
        arma::dmat J_;
        arma::dvec residuals_;
        arma::dvec coeff_;

        void ComputeJacobian(const arma::dvec& coeff);
        void ComputeResiduals(const arma::dvec& coeff);

};
#endif  // TECH_TRACK_API_NLSS_SOLVER_H_

