// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_
#define TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_

#include <armadillo>

class DataHandler{
 protected:
    arma::mat data_;
 public:
    DataHandler(int m, int n);
    virtual ~DataHandler();

    arma::mat GetData() { return data_; }
    // TODO(sec05): Methods for getting data from API and turning it into a matrix
};

#endif  // TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_
