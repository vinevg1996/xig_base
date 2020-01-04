#include "aig.h"

#include <cstdlib>
#include <ctime>
#include <cmath>
#include <set>

// Precessors
Precessors::Precessors(int first_prec,
                       bool is_negation_first_prec,
                       int second_prec,
                       bool is_negation_second_prec,
                       bool variable_flag)
       : first_prec(first_prec)
       , is_negation_first_prec(is_negation_first_prec == 1)
       , second_prec(second_prec)
       , is_negation_second_prec(is_negation_second_prec == 1) 
       , variable_flag(variable_flag) {}

bool Precessors::IsNegationPrec() const {
    return is_negation_first_prec * is_negation_second_prec;
}

bool Precessors::IsPositivePrec() const {
    return (is_negation_first_prec == false) * (is_negation_second_prec == false);
}

bool Precessors::IsCrossPrec() const {
    return (IsNegationPrec() == false) && (IsPositivePrec() == false);
}

bool Precessors::IsVariable() const {
    return variable_flag;
}

bool Precessors::IsConjunction() const {
    return !IsVariable();
}

void Precessors::PrintPrecessors() const {
    std::cout << first_prec << ' ' << is_negation_first_prec
    << ' ' << second_prec << ' ' << is_negation_second_prec
    << std::endl;
}

// AIG
MyAIG::MyAIG() {}

MyAIG::MyAIG(std::ifstream &in_file) {
    ReadHeader(in_file);
}

/*
MyAIG::MyAIG(const std::string &inputFile) {
    std::ifstream aig_in(inputFile);
    ReadHeader(aig_in);
}
*/

void MyAIG::ReadHeader(std::ifstream &aig_in) {
    // mincode
    std::string str;
    std::getline(aig_in, str);
    this->mincode = std::stoi(str);
    ConvertMincodeToVectorInt(str, this->table_of_values);
    str.clear();
    // aig
    std::getline(aig_in, str);
    str.clear();
    // complexity
    std::getline(aig_in, str);
    this->complexity = std::stoi(str);
    str.clear();
    // out
    std::getline(aig_in, str);
    std::vector<int> out_vec;
    ConvertStrToVectorInt(str, 0, out_vec);
    this->out_number = out_vec[0];
    this->negation_out_number = (out_vec[1] == 1);
    str.clear();
    out_vec.clear();
    // circuit
    std::getline(aig_in, str);
    ConvertStrToVectorInt(str, 0, out_vec);
    for (int i = 0; i < 5; ++i) {
        Precessors curr_prec(0, false, 0, false, true);
        circuit_matrix.push_back(curr_prec);
    }
    for (int i = 0; i < this->complexity; ++i) {
        Precessors curr_prec(out_vec[4 * i], out_vec[4 * i + 1], out_vec[4 * i + 2], out_vec[4 * i + 3], false);
        circuit_matrix.push_back(curr_prec);
    }
}

void MyAIG::ConvertMincodeToVectorInt(const std::string &str, std::vector<int> &vec) {
    int curr_mincode = std::stoi(str);
    table_of_values.resize(32, 0);
    for (int i = 31; i >= 0; --i) {
        if (curr_mincode >= std::pow(2, i)) {
            table_of_values[i] = 1;
            curr_mincode = curr_mincode - std::pow(2, i);
        }
    }
}

void MyAIG::ConvertStrToVectorInt(const std::string &str, int startIndex, std::vector<int> &vec) {
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

int64_t MyAIG::GetMincode() const {
    return mincode;
}

int64_t MyAIG::GetComplexity() const {
    return complexity;
}

void MyAIG::PrintAIG() const {
    std::cout << "mincode = " << mincode << std::endl;
    std::cout << "complexity = " << complexity << std::endl;
    std::cout << "out_number = " << out_number << std::endl;
    std::cout << "negation_out_number = " << negation_out_number << std::endl;
    std::cout << "table_of_values:" << std::endl;
    for (int i = 0; i < table_of_values.size(); ++i) {
        if ((i != 0) && (i % 4 == 0)) {
            std::cout << ' ';
        }
        std::cout << table_of_values[i];
    }
    std::cout << std::endl;
    for (int i = 0; i < complexity + 5; ++i) {
        if (circuit_matrix[i].IsVariable()) {
            std::cout << i << ": " << "x" << i << std::endl;
        } else {
            std::cout << i << ": ";
            circuit_matrix[i].PrintPrecessors();
        }
    }
}

void MyAIG::CountXorStructures(int &sum, std::vector<std::vector<int>> &xorStructures) const {
    sum = 0;
    for (int i = 5; i < complexity + 5; ++i) {
        if (circuit_matrix[i].IsNegationPrec()) {
            int prec_1 = circuit_matrix[i].first_prec;
            int prec_2 = circuit_matrix[i].second_prec;
            if ((circuit_matrix[prec_1].IsConjunction()) &&
                (circuit_matrix[prec_2].IsConjunction())) {
                int prec1_root1 = circuit_matrix[prec_1].first_prec;
                int prec1_root2 = circuit_matrix[prec_1].second_prec;
                std::set<int> prec1_set;
                prec1_set.insert(prec1_root1);
                prec1_set.insert(prec1_root2);
                int prec2_root1 = circuit_matrix[prec_2].first_prec;
                int prec2_root2 = circuit_matrix[prec_2].second_prec;
                auto it1 = prec1_set.find(prec2_root1);
                auto it2 = prec1_set.find(prec2_root2);
                if ((it1 != prec1_set.end()) && (it2 != prec1_set.end())) {
                    if (IsXorStruct1(prec_1, prec_2) || IsXorStruct2(prec_1, prec_2)) {
                        ++sum;
                        std::vector<int> xorStructure = {i, prec_1, prec_2};
                        xorStructures.push_back(xorStructure);
                    }
                }
            }
        }
    }
}

bool MyAIG::IsXorStruct1(int prec_1, int prec_2) const {
    if ((circuit_matrix[prec_1].IsPositivePrec() && circuit_matrix[prec_2].IsNegationPrec()) || 
        (circuit_matrix[prec_1].IsNegationPrec() && circuit_matrix[prec_2].IsPositivePrec())) {
        return true;
    }
    return false;
}

bool MyAIG::IsXorStruct2(int prec_1, int prec_2) const {
    if (circuit_matrix[prec_1].IsCrossPrec() && circuit_matrix[prec_2].IsCrossPrec()) {
        return true;
    }
    return false;
}

/*
std::ostream& operator<< (std::ostream &out, const MyAIG &aig) {
    out << "F " << 1 << std::endl;
    out << "s " << fsm.statesNumber << std::endl;
    out << "i " << fsm.inputsNumber << std::endl;
    out << "o " << fsm.outputsNumber << std::endl;
    out << "n0";
    for (int i = 0; i < fsm.initialStateVector.size(); ++i) {
        out << ' ' << fsm.initialStateVector[i];
    }
    out << std::endl << "p " << fsm.transitionsNumber << std::endl;
    for (unsigned int s = 0; s < fsm.statesNumber; s++) {
        for (unsigned int i = 0; i < fsm.inputsNumber; i++) {
            for (unsigned int t = 0; t < fsm.stateTable[s][i].size(); t++) {
                out << s << ' ' << i << ' ' << fsm.stateTable[s][i][t].nextState<< ' '
                << fsm.stateTable[s][i][t].output<< std::endl;
            }
        }
    }
    return out;
}
*/
