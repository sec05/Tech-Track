#ifndef LSTM_H_
#define LSTM_H_

#include <armadillo>

class LSTM {
public:
    LSTM(int input_size, int hidden_size, int output_steps, double lr);

    void train(const arma::cube& X, const arma::cube& Y, int epochs);
    arma::mat predict(const arma::mat& input_seq);

private:
    int input_size, hidden_size, output_steps;
    double lr;

    // Weights
    arma::mat Wf, Wi, Wc, Wo;
    arma::mat Uf, Ui, Uc, Uo;
    arma::vec bf, bi, bc, bo;

    arma::mat Wy;
    arma::vec by;

    // Activations
    arma::mat sigmoid(const arma::mat& x);
    arma::mat dsigmoid(const arma::mat& x);
    arma::mat tanh_act(const arma::mat& x);
    arma::mat dtanh(const arma::mat& x);

    // Forward step
    void forward(const arma::mat& x_seq, std::vector<arma::mat>& h_list,
                 std::vector<arma::mat>& c_list, arma::mat& y_pred);

    // Backward pass
    void backward(const arma::mat& x_seq, const arma::mat& y_true,
                  const std::vector<arma::mat>& h_list,
                  const std::vector<arma::mat>& c_list,
                  const arma::mat& y_pred);
};

#endif
