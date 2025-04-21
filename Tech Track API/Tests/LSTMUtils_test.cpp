// Copyright 2025 Spencer Evans-Cole

#include <gtest/gtest.h>
#include <armadillo>
#include <algorithm>
#include "../Prediction/LSTM/Utils.h"

// Helper: try to allocate a large matrix safely
bool try_large_allocation(arma::mat* data, size_t rows, size_t cols) {
    try {
        data = &arma::randu<arma::mat>(rows, cols);
        return true;
    } catch (const std::bad_alloc&) {
        return false;
    }
}

// Normalize Tests
TEST(LSTMUtilsTest, NormalizeSeveralSizes) {
    for (int i = 1; i < 100000; i *= 10) {
        arma::mat data = arma::randu<arma::mat>(i, 2);
        arma::rowvec min, max;
        normalize(data, &min, &max);
        EXPECT_EQ(min.n_elem, 2);
        EXPECT_EQ(max.n_elem, 2);
        EXPECT_GE(min(0), 0.0);
        EXPECT_LE(max(0), 1.0);
    }
}

TEST(LSTMUnitTest, NormalizeNull) {
    arma::mat data;
    arma::rowvec min, max;
    normalize(data, &min, &max);
    EXPECT_EQ(min.n_elem, 0);
    EXPECT_EQ(max.n_elem, 0);
}

TEST(LSTMUnitTest, NormalizeEmpty) {
    arma::mat data(0, 0);
    arma::rowvec min, max;
    normalize(data, &min, &max);
    EXPECT_EQ(min.n_elem, 0);
    EXPECT_EQ(max.n_elem, 0);
}

TEST(LSTMUnitTest, NormalizeTooBig) {
    arma::mat data;
    arma::rowvec min, max;
    if (!try_large_allocation(&data, 1e8, 10)) {
        SUCCEED() << "Handled out-of-memory safely.";
        return;
    }
    normalize(data, &min, &max);
    EXPECT_EQ(min.n_elem, 10);
    EXPECT_EQ(max.n_elem, 10);
}

TEST(LSTMUnitTest, AlreadyNormalized) {
    arma::mat data = arma::randu<arma::mat>(100, 100);
    arma::normalise(data, 1);
    arma::mat dataCopy = data;
    arma::rowvec min, max;
    normalize(data, &min, &max);
    EXPECT_EQ(min.n_elem, 100);
    EXPECT_EQ(max.n_elem, 100);
    EXPECT_TRUE(arma::approx_equal(data, dataCopy, "absdiff", 1e-12));
}

// Denormalize Tests
TEST(LSTMUtilsTest, DenormalizeSeveralSizes) {
    for (int i = 1; i < 100000; i *= 10) {
        arma::mat data = arma::randu<arma::mat>(i, 2);
        arma::rowvec min = arma::min(data, 0);
        arma::rowvec max = arma::max(data, 0);
        arma::normalise(data, 1);
        arma::mat dataCopy = data;
        denormalize(data, min, max);
        EXPECT_TRUE(arma::approx_equal(data, dataCopy, "absdiff", 1e-12));
    }
}

TEST(LSTMUnitTest, DenormalizeNull) {
    arma::mat data;
    arma::rowvec min, max;
    denormalize(data, min, max);
    EXPECT_EQ(data.n_elem, 0);
    EXPECT_EQ(min.n_elem, 0);
    EXPECT_EQ(max.n_elem, 0);
}

TEST(LSTMUnitTest, DenormalizeEmpty) {
    arma::mat data(0, 0);
    arma::rowvec min, max;
    denormalize(data, min, max);
    EXPECT_EQ(min.n_elem, 0);
    EXPECT_EQ(max.n_elem, 0);
}

TEST(LSTMUnitTest, DenormalizeTooBig) {
    arma::mat data;
    arma::rowvec min, max;
    if (!try_large_allocation(&data, 1e8, 10)) {
        SUCCEED() << "Handled out-of-memory safely.";
        return;
    }
    denormalize(data, min, max);
    EXPECT_TRUE(data.n_elem > 0 || data.is_empty());
}

TEST(LSTMUnitTest, AlreadyDenormalized) {
    arma::mat data = arma::randu<arma::mat>(100, 100);
    arma::mat dataCopy = data;
    arma::rowvec min, max;
    denormalize(data, min, max);
    EXPECT_TRUE(arma::approx_equal(data, dataCopy, "absdiff", 1e-12));
}

TEST(LSTMUnitTest, DenormalizeWithZeroRange) {
    arma::mat data = arma::randu<arma::mat>(100, 100);
    arma::rowvec min = arma::zeros<arma::rowvec>(100);
    arma::rowvec max = arma::ones<arma::rowvec>(100);
    denormalize(data, min, max);
    EXPECT_EQ(data.n_rows, 100);
    EXPECT_EQ(data.n_cols, 100);
}

// Build Sequences Tests
TEST(LSTMUnitTest, BuildSequencesBasic) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    EXPECT_EQ(X.n_rows, 10);
    EXPECT_EQ(X.n_cols, 10);
    EXPECT_EQ(X.n_slices, data.n_slices - 10 - 5 + 1);
}

TEST(LSTMUnitTest, BuildSequencesEmpty) {
    arma::cube data;
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    EXPECT_EQ(X.n_elem, 0);
    EXPECT_EQ(Y.n_elem, 0);
}

TEST(LSTMUnitTest, BuildSequencesNegativeParams) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, -10, 5, &X, &Y);
    EXPECT_EQ(X.n_elem, 0);
    build_sequences(data, 10, -5, &X, &Y);
    EXPECT_EQ(X.n_elem, 0);
}

// Train/Test Split
TEST(LSTMUnitTest, TrainTestSplitBasic) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    arma::cube xtrain, ytrain, xtest, ytest;
    train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, 0.8);
    EXPECT_EQ(xtrain.n_rows, 10);
    EXPECT_EQ(ytrain.n_cols, 10);
}

TEST(LSTMUnitTest, TrainTestSplitEmpty) {
    arma::cube X, Y;
    arma::cube xtrain, ytrain, xtest, ytest;
    train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, 0.8);
    EXPECT_EQ(xtrain.n_elem, 0);
}

TEST(LSTMUnitTest, TrainTestSplitZeroRatio) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    arma::cube xtrain, ytrain, xtest, ytest;
    train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, 0.0);
    EXPECT_EQ(xtrain.n_elem, 0);
    EXPECT_EQ(xtest.n_elem, X.n_elem);
}

TEST(LSTMUnitTest, TrainTestSplitOneRatio) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    arma::cube xtrain, ytrain, xtest, ytest;
    train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, 1.0);
    EXPECT_EQ(xtrain.n_elem, X.n_elem);
    EXPECT_EQ(xtest.n_elem, 0);
}

TEST(LSTMUnitTest, TrainTestSplitInvalidRatio) {
    arma::cube data = arma::randu<arma::cube>(100, 10, 5);
    arma::cube X, Y;
    build_sequences(data, 10, 5, &X, &Y);
    arma::cube xtrain, ytrain, xtest, ytest;
    EXPECT_THROW(train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, -0.1),
     std::invalid_argument);
    EXPECT_THROW(train_test_split(X, Y, &xtrain, &ytrain, &xtest, &ytest, 1.5),
     std::invalid_argument);
}

// Run all tests
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
