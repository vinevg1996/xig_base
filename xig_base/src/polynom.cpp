#include "basic_circuit.h"
#include "polynom.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <cmath>

Monom::Monom(const std::vector<int> &monom) 
        : monom(monom) {}

Monom::Monom(const Monom &other_monom)
        : monom(other_monom.monom) {}

bool Monom::IsNullMonom() const {
    return ((monom[0] == 0) && (monom[1] == 0) && (monom[2] == 0) &&
           (monom[3] == 0) && (monom[4] == 0));
}

bool Monom::IsVariable() const {
    int var_number = 0;
    for (const auto &elem: this->monom) {
        if (elem) {
            ++var_number;
        }
    }
    return (var_number == 1);
}

int Monom::GetSingleVariable() const {
    for (int i = 0; i < this->monom.size(); ++i) {
        if (this->monom[i]) {
            return i;
        }
    }
}

int Monom::ComputeValue(const std::vector<int> &var_values) const {
    for (int i = 0; i < this->monom.size(); ++i) {
        if ((this->monom[i]) && (var_values[i] == 0)) {
            return 0;
        }
    }
    return 1;
}

void Monom::ComputeTableOfValues(std::vector<int> &computed_table_of_values) const {
    int size = 32;
    computed_table_of_values.resize(size, 0);
    std::vector<int> var_values;
    for (int i = 0; i < size; ++i) {
        ConvertMincodeToVectorInt(i, 5, var_values);
        computed_table_of_values[i] = this->ComputeValue(var_values);
        var_values.clear();
        //std::cout << computed_table_of_values[i] << ' ';
    }
    //std::cout << std::endl;
}

int Monom::CountVariables() const {
    int var_number = 0;
    for (const auto &elem: this->monom) {
        if (elem) {
            ++var_number;
        }
    }
    return var_number;
}

int Monom::CountComplexity() const {
    int var_number = 0;
    for (const auto &elem: this->monom) {
        if (elem) {
            ++var_number;
        }
    }
    if (var_number >= 2) {
        return var_number - 1;
    } else {
        return 0;
    }
}

void Monom::Print() const {
    if (this->IsNullMonom()) {
        //std::cout << "1" << std::endl;
        std::cout << "1";
    } else {
        for (int i = 0; i < monom.size(); ++i) {
            if (monom[i]) {
                std::cout << "x_" << i + 1;
            }
        }
    }
}

// Poly
void Poly::ConvertTableToPolynomCoeff() {
    poly_coeff = table_of_values;
    int size = poly_coeff.size();
    int log_2_size = 5;
    for (int curr_pow = 1; curr_pow <= log_2_size; ++curr_pow) {
        int part_size = std::pow(2, curr_pow);
        int half_part_size = part_size / 2;
        int part_number = size / part_size;
        for (int part = 0; part < part_number; ++part) {
            for (int k = 0; k < half_part_size; ++k) {
                poly_coeff.at(part * part_size + half_part_size + k) = (poly_coeff.at(part * part_size + k) + poly_coeff.at(part * part_size + half_part_size + k)) % 2;
            }
        }
    }
}

Poly::Poly(const BasicCircuit &basic)
        : mincode(basic.GetMincode())
        , basic_complexity(basic.GetComplexity())
        , table_of_values(basic.GetTableOfValues()) {
    ConvertTableToPolynomCoeff();
    std::vector<int> curr_monom;
    for (int i = 0; i < poly_coeff.size(); ++i) {
        if (poly_coeff[i]) {
            //std::cout << "i = " << i << std::endl;
            ConvertMincodeToVectorInt(i, 5, curr_monom);
            Monom monom(curr_monom);
            polynom.push_back(monom);
            curr_monom.clear();
        }
    }
}

bool Poly::IsOnePolynom() const {
    return ((this->polynom.size() == 1) && 
            (this->polynom[0].IsNullMonom()));
}

int Poly::ComputeValue(const std::vector<int> &var_values) const {
    int value = 0;
    for (const auto &curr_monom: this->polynom) {
        value = (value + curr_monom.ComputeValue(var_values)) % 2;
    }
    return value;
}

void Poly::ComputeTableOfValues(std::vector<int> &computed_table_of_values) const {
    //std::cout << "Poly::ComputeTableOfValues:" << std::endl;
    int size = 32;
    computed_table_of_values.resize(size, 0);
    std::vector<int> var_values;
    for (int i = 0; i < size; ++i) {
        ConvertMincodeToVectorInt(i, 5, var_values);
        computed_table_of_values[i] = this->ComputeValue(var_values);
        var_values.clear();
        //std::cout << computed_table_of_values[i] << ' ';
    }
    //std::cout << std::endl;
}

int Poly::CountComplexity() const {
    // is exist 1
    int complexity = 0;
    if (this->polynom.size() > 0) {
        if (!this->polynom[0].IsNullMonom()) {
            complexity = this->polynom[0].CountComplexity();
            ++complexity;
        }
        for (int i = 1; i < this->polynom.size(); ++i) {
            complexity += this->polynom[i].CountComplexity();
            if (i < this->polynom.size() - 1) {
                ++complexity;
            }
        }
        return complexity;
    } else {
        return 0;
    }
}

void Poly::Print() const {
    //std::cout << "Polynom:" << std::endl;
    if (polynom.size() > 0) {
        for (int i = 0; i < polynom.size() - 1; ++i) {
            polynom[i].Print();
            std::cout << " + ";
        }
        polynom[polynom.size() - 1].Print();
        //std::cout << std::endl;
    } else {
        std::cout << "0";
    }
}
