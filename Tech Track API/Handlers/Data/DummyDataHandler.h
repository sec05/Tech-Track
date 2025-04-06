// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_HANDLERS_DATA_DUMMYDATAHANDLER_H_
#define TECH_TRACK_API_HANDLERS_DATA_DUMMYDATAHANDLER_H_

#include <armadillo>

#include "DataHandler.h"

class DummyDataHandler : public DataHandler{
 public:
    DummyDataHandler(int m, int n);
    ~DummyDataHandler() override;
    arma::vec LeastSquaresPreditct(int days);
    arma::vec GetDataVector(int column);
};


#endif  // TECH_TRACK_API_HANDLERS_DATA_DUMMYDATAHANDLER_H_
