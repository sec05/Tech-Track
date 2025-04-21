// Copyright 2025 Spencer Evans-Cole

#include "LSTMDriver.h"
#include "Utils.h"
#include <iostream>
#include <mutex>
LSTMDriver::LSTMDriver(const arma::dmat& data_matrix, int seq_len, int pred_len,
                       int hidden_size, int epochs, double lr, double train_ratio)
    : data(data_matrix), T(seq_len), N(pred_len), lstm(0, hidden_size, pred_len, lr) {
    std::cout << "LSTMDriver constructor called" << std::endl;
    // Load + normalize
    F = data.n_cols;
    lstm = LSTM(F, hidden_size, N, lr);
    normalize(data, &min, &max);

    // Build sequences
    arma::cube X, Y;
    build_sequences(data, T, N, &X, &Y);

    // Split train/test
    train_test_split(X, Y, &X_train, &Y_train, &X_test, &Y_test, train_ratio);

    // Train model
    lstm.train(X_train, Y_train, epochs);
}

arma::mat LSTMDriver::predict_from_last() {
    arma::mat last_seq = data.tail_rows(5);  // last T rows
    arma::rowvec dummy_min = min, dummy_max = max;

    arma::mat norm_last_seq = arma::normalise(last_seq, 1);
    arma::mat prediction = lstm.predict(norm_last_seq);

    prediction = prediction.t();
    prediction.reshape(N, F);
    denormalize(prediction, min, max);

    return prediction;
}

LSTMDriver* LSTMDriver::instance = nullptr;
std::once_flag LSTMDriver::initFlag;

void LSTMDriver::init(const arma::dmat& data_matrix, int seq_len, int pred_len,
                      int hidden_size, int epochs, double lr, double train_ratio) {
    std::call_once(initFlag, [&]() {
        instance = new LSTMDriver(data_matrix, seq_len, pred_len, hidden_size, epochs,
            lr, train_ratio);
    });
}

LSTMDriver& LSTMDriver::getInstance() {
    if (!instance) {
        throw std::runtime_error("LSTMDriver is not initialized. Call init() first.");
    }
    return *instance;
}
