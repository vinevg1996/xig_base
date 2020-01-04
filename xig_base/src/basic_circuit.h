#ifndef BASIC_FUNCTION
#define BASIC_FUNCTION

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>

void ConvertMincodeToVectorInt(int curr_mincode, int size, std::vector<int> &vec);

void ConvertStrToVectorInt(const std::string &str, int startIndex, std::vector<int> &vec);

class BasicCircuit {
    int64_t mincode;
    int64_t complexity;
    std::vector<int> table_of_values;
    std::string out_string;
    std::string vertex_string;

public:
    BasicCircuit() {}
    BasicCircuit(std::ifstream &in_file);

    int64_t GetMincode() const {
        return mincode;
    }
    int64_t GetComplexity() const {
        return complexity;
    }
    std::vector<int> GetTableOfValues() const {
        return table_of_values;
    }
    void WriteCircuitToFile(std::ofstream &out_file);
};

#endif