// Copyright 2025 Spencer Evans-Cole

#ifndef TECH_TRACK_API_PREDICTION_LSTM_UTILS_H_
#define TECH_TRACK_API_PREDICTION_LSTM_UTILS_H_

#include <armadillo>
#include <vector>

// min and max are output parameters → now pointers
void normalize(arma::mat& data, arma::rowvec* min, arma::rowvec* max);

// min and max are const inputs, so keep as const references
void denormalize(arma::mat& data, const arma::rowvec& min, const arma::rowvec& max);

// X and Y are output sequences → now pointers
void build_sequences(const arma::mat& data, int seq_len, int pred_len,
                     arma::cube* X, arma::cube* Y);

// X_train, Y_train, X_test, Y_test are outputs → now pointers
void train_test_split(const arma::cube& X, const arma::cube& Y,
                      arma::cube* X_train, arma::cube* Y_train,
                      arma::cube* X_test, arma::cube* Y_test,
                      double train_ratio);

#endif  // TECH_TRACK_API_PREDICTION_LSTM_UTILS_H_
