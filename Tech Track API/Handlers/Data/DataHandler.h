// Copyright 2025 Spencer Evans-Cole
#ifndef TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_
#define TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_

#include <armadillo>
#include <vector>
#include <string>
struct PollingCube {
   arma::cube data;                              // shape: [techs x endpoints x dates]
   std::vector<std::string> tech_labels;         // row labels
   std::vector<std::string> endpoint_labels;     // column labels
   std::vector<std::string> date_labels;         // slice labels
};
class DataHandler{
 private:
    PollingCube data_cube_;
 public:
    DataHandler(int m, int n);
    virtual ~DataHandler();

    PollingCube GetDataFromPolling();
    PollingCube FillRandomPollingCube(size_t num_techs, size_t num_endpoints, size_t num_dates);
    arma::mat GetMatrixByDateAndTech();
    arma::vec GetDataByEndpointAndTech(const std::string& endpoint, const std::string& tech);
    std::vector<std::string>GetTechLabels() { return data_cube_.tech_labels; }
      std::vector<std::string>GetEndpointLabels() { return data_cube_.endpoint_labels; }
      std::vector<std::string>GetDateLabels() { return data_cube_.date_labels; }
};

#endif  // TECH_TRACK_API_HANDLERS_DATA_DATAHANDLER_H_
