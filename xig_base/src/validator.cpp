#include "validator.h"

#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <set>
#include <memory>

Validator::Validator(const std::shared_ptr<Function> &func,
                     const std::vector<Precessors> &circuit_matrix,
                     int out_vertex,
                     bool out_neg_flag,
                     int64_t mincode) 
        : func(func) 
        , circuit_matrix(circuit_matrix) 
        , out_vertex(out_vertex)
        , out_neg_flag(out_neg_flag) 
        , mincode(mincode) 
        , complexity(circuit_matrix.size() - 5) {}

std::vector<int> Validator::BuildXorFunction(const std::vector<int> &table1, const std::vector<int> &table2) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] + table2[i]) % 2;
    }
    return new_table_of_values;
}

std::vector<int> Validator::BuildMultFunction(const std::vector<int> &table1, const std::vector<int> &table2) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] * table2[i]) % 2;
    }
    return new_table_of_values;
}

std::vector<int> Validator::BuildNotFunction(const std::vector<int> &table1) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] + 1) % 2;
    }
    return new_table_of_values;
}

// for index more than 4
std::vector<int> Validator::ComputeTableOfValuesForVertex(int vertex_number) const {
    std::vector<int> table1 = circuit_matrix_table_of_values[circuit_matrix[vertex_number].first_prec];
    std::vector<int> table2 = circuit_matrix_table_of_values[circuit_matrix[vertex_number].second_prec];
    int neg_prec1 = circuit_matrix[vertex_number].gate_and_first_prec_negation;
    int neg_prec2 = circuit_matrix[vertex_number].gate_and_second_prec_negation;
    if (neg_prec1 % 2) {
        table1 = BuildNotFunction(table1);
    }
    if (neg_prec2 % 2) {
        table2 = BuildNotFunction(table2);
    }
    if ((neg_prec1 / 2 == 0) && (neg_prec2 / 2 == 0)) {
        return BuildMultFunction(table1, table2);
    }
    if ((neg_prec1 / 2 == 1) && (neg_prec2 / 2 == 1)) {
        return BuildXorFunction(table1, table2);
    }
}

bool Validator::Validate() {
    std::vector<int> null_vector(32, 0);
    circuit_matrix_table_of_values.resize(5, null_vector);
    func->ComputeTableOfValues(func_table_of_value);
    std::vector<int> monom_vec = {0, 0, 0, 0, 0};
    Monom monom(monom_vec);
    monom.monom[0] = 1;
    monom.ComputeTableOfValues(circuit_matrix_table_of_values[0]);
    monom.monom[0] = 0;
    monom.monom[1] = 1;
    monom.ComputeTableOfValues(circuit_matrix_table_of_values[1]);
    monom.monom[1] = 0;
    monom.monom[2] = 1;
    monom.ComputeTableOfValues(circuit_matrix_table_of_values[2]);
    monom.monom[2] = 0;
    monom.monom[3] = 1;
    monom.ComputeTableOfValues(circuit_matrix_table_of_values[3]);
    monom.monom[3] = 0;
    monom.monom[4] = 1;
    monom.ComputeTableOfValues(circuit_matrix_table_of_values[4]);

    // TODO: Problem
    for (int i = 5; i < circuit_matrix.size(); ++i) {
        std::vector<int> curr_vertex_value;
        curr_vertex_value = ComputeTableOfValuesForVertex(i);
        circuit_matrix_table_of_values.push_back(curr_vertex_value);
    }
    std::vector<int> compute_table = circuit_matrix_table_of_values[circuit_matrix_table_of_values.size() - 1];
    if (out_neg_flag) {
        BuildNotFunction(compute_table);
    }
    if (func_table_of_value == compute_table) {
        return true;
    }
    return false;
}

void Validator::PrintCircuitMatrixValue() const {
    for (const auto &curr_vec: circuit_matrix_table_of_values) {
        for (const auto &elem: curr_vec) {
            std::cout << elem << ' ';
        }
        std::cout << std::endl;
    }
}

void Validator::WriteCircuitToFile(std::ofstream &out_file) {
    out_file << mincode << std::endl;
    out_file << "xig" << std::endl;
    out_file << complexity << std::endl;
    out_file << out_vertex + 1 << ' ' << out_neg_flag << std::endl;
    for (int i = 5; i < circuit_matrix.size(); ++i) {
        out_file << circuit_matrix[i].first_prec + 1 << ' '
        << circuit_matrix[i].gate_and_first_prec_negation << ' '
        << circuit_matrix[i].second_prec + 1 << ' '
        << circuit_matrix[i].gate_and_second_prec_negation << ' ';
    }
    out_file << std::endl;
}
