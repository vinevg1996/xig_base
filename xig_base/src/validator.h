#ifndef VALIDATOR
#define VALIDATOR

#include "implementation.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

class Validator {
    // out and neg flag
    std::shared_ptr<Function> func;
    std::vector<int> func_table_of_value;
    std::vector<Precessors> circuit_matrix;
    std::vector<std::vector<int>> circuit_matrix_table_of_values;
    int out_vertex;
    bool out_neg_flag;
    int64_t mincode;
    int complexity;

    std::vector<int> BuildXorFunction(const std::vector<int> &table1, const std::vector<int> &table2) const;
    std::vector<int> BuildMultFunction(const std::vector<int> &table1, const std::vector<int> &table2) const;
    std::vector<int> BuildNotFunction(const std::vector<int> &table1) const;
    std::vector<int> ComputeTableOfValuesForVertex(int vertex_number) const;

public:
    Validator(const std::shared_ptr<Function> &func,
              const std::vector<Precessors> &circuit_matrix,
              int out_vertex,
              bool out_neg_flag,
              int64_t mincode);

    bool Validate();
    int GetComplexity() const {
        return complexity;
    }

    void PrintCircuitMatrixValue() const;
    void WriteCircuitToFile(std::ofstream &out_file);
};

#endif