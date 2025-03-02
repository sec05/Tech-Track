// Copyright 2025 Spencer Evans-Cole
#include "DummyDataHandler.h"

#include <armadillo>

#include "DataHandler.h"

DummyDataHandler::DummyDataHandler(int m, int n) : DataHandler(m, n) {
    data_ = arma::mat(m, n, arma::fill::zeros);
    for (int i = 0; i < m; i++) {
        data_(i, 0) = i+1;
        for (int j = 1; j < n; j++) {
            data_(i, j) = random()*1000;
        }
    }
    data_.save("data.csv", arma::csv_ascii);
}

DummyDataHandler::~DummyDataHandler() {
}

arma::vec DummyDataHandler::LeastSquaresPreditct(int days) {
    int m = data_.n_rows;
    int days_in_future = m + days;
    arma::mat X = arma::mat(m, 2, arma::fill::ones);
    arma::vec y = data_.col(1);
    for (int i = 0; i < m; i++) {
        X(i, 1) = data_(i, 0);
    }
    arma::vec beta = arma::solve(X, y);
    arma::vec future = arma::vec(days_in_future, arma::fill::ones);
    for (int i = m; i < days_in_future; i++) {
        future(i) = beta(0) + beta(1)*i;
    }
    return future;
}

arma::vec DummyDataHandler::GetDataVector(int column) {
    return data_.col(column);
}

