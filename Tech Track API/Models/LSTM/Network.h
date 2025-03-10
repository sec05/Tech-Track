// Copyright 2025 Sprencer Evans-Cole
#ifndef TECH_TRACK_API_MODELS_LSTM_NETWORK_H_
#define TECH_TRACK_API_MODELS_LSTM_NETWORK_H_

#include <armadillo>

using namespace arma;

class LSTMNetwork {
 public:
        LSTMNetwork(int input_size, int hidden_size, int output_size);
        ~LSTMNetwork();
        void Forward(const dmat& input);
        dvec GetOutput() const { return hidden_state_; }
 private:
        int input_size_;
        int hidden_size_;
        int output_size_;

        dmat output_weights_;
        dmat input_weights_;
        dmat cell_weights_;
        dmat forget_weights_;

        dvec forget_bias_;
        dvec input_bias_;
        dvec output_bias_;
        dvec cell_bias_;

        dvec hidden_state_;
        dvec cell_state_;
};
#endif  // TECH_TRACK_API_MODELS_LSTM_NETWORK_H_
