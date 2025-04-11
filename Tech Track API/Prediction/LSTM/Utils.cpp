// Copyright 2025 Spencer Evans-Cole

#include "Utils.h"

void normalize(arma::mat& data, arma::rowvec* min, arma::rowvec* max) {
    *min = arma::min(data, 0);
    *max = arma::max(data, 0);
    data = (data.each_row() - *min) / (*max - *min + 1e-8);
}

void denormalize(arma::mat& data, const arma::rowvec& min, const arma::rowvec& max) {
    data = data.each_row() % (max - min + 1e-8) + min;
}

void build_sequences(const arma::mat& data, int seq_len, int pred_len,
                     arma::cube* X, arma::cube* Y) {
    int N = data.n_rows - seq_len - pred_len + 1;
    int F = data.n_cols;

    X->set_size(seq_len, F, N);
    Y->set_size(pred_len, F, N);

    for (int i = 0; i < N; ++i) {
        X->slice(i) = data.rows(i, i + seq_len - 1);
        Y->slice(i) = data.rows(i + seq_len, i + seq_len + pred_len - 1);
    }
}

void train_test_split(const arma::cube& X, const arma::cube& Y,
                      arma::cube* X_train, arma::cube* Y_train,
                      arma::cube* X_test, arma::cube* Y_test,
                      double train_ratio) {
    int N = X.n_slices;
    int train_size = static_cast<int>(N * train_ratio);

    *X_train = X.slices(0, train_size - 1);
    *Y_train = Y.slices(0, train_size - 1);
    *X_test  = X.slices(train_size, N - 1);
    *Y_test  = Y.slices(train_size, N - 1);
}
