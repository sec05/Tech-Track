// Copyright 2025 Spencer Evans-Cole
#include "Network.h"
#include <armadillo>

double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

LSTMNetwork::LSTMNetwork(int input_size, int hidden_size, int output_size)
    : input_size_(input_size), hidden_size_(hidden_size), output_size_(output_size) {
    // Initialize weights and biases
    input_weights_ = arma::randu<dmat>(input_size_, hidden_size_);
    forget_weights_ = arma::randu<dmat>(input_size_, hidden_size_);
    cell_weights_ = arma::randu<dmat>(input_size_, hidden_size_);
    output_weights_ = arma::randu<dmat>(input_size_, hidden_size_);

    input_bias_ = arma::randu<dvec>(hidden_size_);
    forget_bias_ = arma::randu<dvec>(hidden_size_);
    cell_bias_ = arma::randu<dvec>(hidden_size_);
    output_bias_ = arma::randu<dvec>(hidden_size_);

    // Initialize hidden and cell states
    hidden_state_ = arma::zeros<dvec>(hidden_size_);
    cell_state_ = arma::zeros<dvec>(hidden_size_);
}
LSTMNetwork::~LSTMNetwork() {
    // Destructor
}

void LSTMNetwork::Forward(const dmat& input) {
    // Forward pass through the LSTM network
    for (int t = 0; t < input.n_rows; ++t) {
        // Input gate
        dvec input_gate = input.row(t) * input_weights_ + input_bias_;
        for (int i = 0; i < input_gate.n_elem; ++i) {
            input_gate(i) = sigmoid(input_gate(i));
        }

        // Forget gate
        dvec forget_gate = input.row(t) * forget_weights_ + forget_bias_;
        for (int i = 0; i < forget_gate.n_elem; ++i) {
            forget_gate(i) = sigmoid(forget_gate(i));
        }

        // Cell state
        dvec cell_input = arma::tanh(input.row(t) * cell_weights_ + cell_bias_);
        cell_state_ = forget_gate % cell_state_ + input_gate % cell_input;

        // Output gate
        dvec output_gate = input.row(t) * output_weights_ + output_bias_;
        for (int i = 0; i < output_gate.n_elem; ++i) {
            output_gate(i) = sigmoid(output_gate(i));
        }

        // Hidden state
        hidden_state_ = output_gate % arma::tanh(cell_state_);
    }
}
