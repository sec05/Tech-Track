// Copyright 2025 Spencer Evans-Cole

#ifndef TECH_TRACK_API_PREDICTION_LSTM_LSTMDRIVER_H_
#define TECH_TRACK_API_PREDICTION_LSTM_LSTMDRIVER_H_

#include <armadillo>
#include <string>
#include <mutex>  // For std::once_flag
#include "LSTM.h"

class LSTMDriver {
 public:
    // Singleton: delete copy constructor and assignment
    LSTMDriver(const LSTMDriver&) = delete;
    LSTMDriver& operator=(const LSTMDriver&) = delete;

    // Get singleton instance
    static LSTMDriver& getInstance();

    // Initialize singleton with constructor args (only first call does anything)
    static void init(const arma::dmat&, int seq_len = 20, int pred_len = 5,
                     int hidden_size = 50, int epochs = 100, double lr = 0.01,
                     double train_ratio = 0.7);

    // Predict next N steps from latest available sequence
    arma::mat predict_from_last();

 private:
    // Private constructor
    explicit LSTMDriver(const arma::dmat&, int seq_len = 20, int pred_len = 5,
                        int hidden_size = 50, int epochs = 100, double lr = 0.01,
                        double train_ratio = 0.7);

    // Data members
    arma::mat data;
    arma::rowvec min, max;
    arma::cube X_train, Y_train, X_test, Y_test;
    LSTM lstm;
    int T, F, N;

    // Singleton internals
    static LSTMDriver* instance;
    static std::once_flag initFlag;
};

#endif  // TECH_TRACK_API_PREDICTION_LSTM_LSTMDRIVER_H_
