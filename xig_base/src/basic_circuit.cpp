#include "basic_circuit.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <cmath>

// Help functions
/*
void ConvertMincodeToVectorInt(int curr_mincode, int size, std::vector<int> &vec) {
    //int curr_mincode = std::stoi(str);
    vec.resize(size, 0);
    for (int i = size - 1; i >= 0; --i) {
        if (curr_mincode >= std::pow(2, i)) {
            vec[i] = 1;
            curr_mincode = curr_mincode - std::pow(2, i);
       }
    }
}
*/

void ConvertMincodeToVectorInt(int curr_mincode, int size, std::vector<int> &vec) {
    //int curr_mincode = std::stoi(str);
    vec.resize(size, 0);
    for (int i = size - 1; i >= 0; --i) {
        if (curr_mincode >= std::pow(2, i)) {
            //vec[i] = 1;
            vec[size - i - 1] = 1;
            curr_mincode = curr_mincode - std::pow(2, i);
       }
    }
}

void ConvertStrToVectorInt(const std::string &str, int startIndex, std::vector<int> &vec) {
    int dig = 0;
    for (int i = startIndex; i < str.size(); ++i) {
        while ((i < str.size()) && (str[i] != ' ')) {
            dig = dig * 10 + (str[i] - '0');
            ++i;
        }
        if (str[i] == ' ') {
            vec.push_back(dig);
            dig = 0;
        }
    }
    vec.push_back(dig);
    dig = 0;
}

// BasicCircuit
BasicCircuit::BasicCircuit(std::ifstream &aig_in) {
    // mincode
    std::string str;
    std::getline(aig_in, str);
    this->mincode = std::stoi(str);
    ConvertMincodeToVectorInt(this->mincode, 32, this->table_of_values);
    str.clear();
    // aig
    std::getline(aig_in, str);
    str.clear();
    // complexity
    std::getline(aig_in, str);
    this->complexity = std::stoi(str);
    str.clear();
    // out
    std::getline(aig_in, out_string);
    std::vector<int> out_vec;
    ConvertStrToVectorInt(str, 0, out_vec);
    //str.clear();
    out_vec.clear();
    // circuit
    std::getline(aig_in, vertex_string);
}

void BasicCircuit::WriteCircuitToFile(std::ofstream &out_file) {
	out_file << mincode << std::endl;
    out_file << "xig" << std::endl;
    out_file << complexity << std::endl;
    for (int i = 0; i < out_string.size(); ++i) {
    	out_file << out_string[i];
    }
    out_file << std::endl;
    for (int i = 0; i < vertex_string.size(); ++i) {
    	out_file << vertex_string[i];
    }
    out_file << std::endl;
}

/*
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
*/
