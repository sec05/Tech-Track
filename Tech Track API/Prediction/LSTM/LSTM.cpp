#include "LSTM.h"
#include <iostream>

LSTM::LSTM(int input_size, int hidden_size, int output_steps, double lr)
    : input_size(input_size), hidden_size(hidden_size),
    output_steps(output_steps), lr(lr) {

    arma::arma_rng::set_seed_random();

    Wf = arma::randn(hidden_size, input_size);
    Wi = arma::randn(hidden_size, input_size);
    Wc = arma::randn(hidden_size, input_size);
    Wo = arma::randn(hidden_size, input_size);

    Uf = arma::randn(hidden_size, hidden_size);
    Ui = arma::randn(hidden_size, hidden_size);
    Uc = arma::randn(hidden_size, hidden_size);
    Uo = arma::randn(hidden_size, hidden_size);

    bf = arma::randn(hidden_size);
    bi = arma::randn(hidden_size);
    bc = arma::randn(hidden_size);
    bo = arma::randn(hidden_size);

    Wy = arma::randn(output_steps * input_size, hidden_size);
    by = arma::randn(output_steps * input_size);
}

// Activation functions
arma::mat LSTM::sigmoid(const arma::mat& x) {
    return 1.0 / (1.0 + arma::exp(-x));
}

arma::mat LSTM::dsigmoid(const arma::mat& x) {
    arma::mat sig = sigmoid(x);
    return sig % (1 - sig);
}

arma::mat LSTM::tanh_act(const arma::mat& x) {
    return arma::tanh(x);
}

arma::mat LSTM::dtanh(const arma::mat& x) {
    return 1 - arma::square(arma::tanh(x));
}

void LSTM::forward(const arma::mat& x_seq, std::vector<arma::mat>& h_list,
    std::vector<arma::mat>& c_list, arma::mat& y_pred) {
    int T = x_seq.n_rows;
    h_list.clear();
    c_list.clear();

    arma::mat h_t = arma::zeros(hidden_size, 1);
    arma::mat c_t = arma::zeros(hidden_size, 1);

    for (int t = 0; t < T; ++t) {
        arma::mat x_t = x_seq.row(t).t();

        arma::mat f_t = sigmoid(Wf * x_t + Uf * h_t + bf);
        arma::mat i_t = sigmoid(Wi * x_t + Ui * h_t + bi);
        arma::mat c_hat = tanh_act(Wc * x_t + Uc * h_t + bc);
        c_t = f_t % c_t + i_t % c_hat;
        arma::mat o_t = sigmoid(Wo * x_t + Uo * h_t + bo);
        h_t = o_t % tanh_act(c_t);

        h_list.push_back(h_t);
        c_list.push_back(c_t);
    }

    y_pred = Wy * h_t + by;
}

void LSTM::backward(const arma::mat& x_seq, const arma::mat& y_true,
    const std::vector<arma::mat>& h_list,
    const std::vector<arma::mat>& c_list,
    const arma::mat& y_pred) {
    arma::mat dy = y_pred - y_true;  // MSE grad

    // Output layer grads
    arma::mat dWy = dy * h_list.back().t();
    arma::vec dby = dy;

    // Apply updates
    Wy -= lr * dWy;
    by -= lr * dby;
}

void LSTM::train(const arma::cube& X, const arma::cube& Y, int epochs) {
    for (int epoch = 0; epoch < epochs; ++epoch) {
        double total_loss = 0.0;
        for (size_t i = 0; i < X.n_slices; ++i) {
            arma::mat x_seq = X.slice(i);
            arma::mat y_true = arma::vectorise(Y.slice(i));  // flatten

            std::vector<arma::mat> h_list, c_list;
            arma::mat y_pred;
            forward(x_seq, h_list, c_list, y_pred);
            arma::mat diff = y_pred - y_true;
            total_loss += arma::accu(diff % diff);

            backward(x_seq, y_true, h_list, c_list, y_pred);
        }
        std::cout << "Epoch " << epoch + 1 << ", Loss: " << total_loss / X.n_slices << std::endl;
    }
}

arma::mat LSTM::predict(const arma::mat& input_seq) {
    std::vector<arma::mat> h_list, c_list;
    arma::mat y_pred;
    forward(input_seq, h_list, c_list, y_pred);
    return y_pred;
}