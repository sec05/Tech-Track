#include "DummyDataGenerator.h"
#include <armadillo>

DummyDataGenerator::DummyDataGenerator(int number_points, int lower_bound, int upper_bound) {
    numberPoints_ = number_points;
    data_ = arma::mat(number_points, 2);
    data_.col(0) = arma::linspace(lower_bound, upper_bound, number_points);
}
DummyDataGenerator::~DummyDataGenerator() {
}

arma::mat DummyDataGenerator::GenerateSinusoidalData(double a, double b, double c, double d, double mean, double stddev) {
    arma::mat data = data_;
    arma::vec noise = arma::randn<arma::vec>(numberPoints_) * stddev + mean;
    data.col(1) = a * arma::sin(b * data.col(0) ) + c * arma::cos(d * data.col(0)) + noise;
    return data;
}

