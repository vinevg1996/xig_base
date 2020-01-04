#ifndef POLYNOM
#define POLYNOM

#include "basic_circuit.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>

struct Monom {
    std::vector<int> monom;

    Monom(const std::vector<int> &monom);
    Monom(const Monom &other_monom);
    bool IsNullMonom() const;
    bool IsVariable() const;
    int GetSingleVariable() const;
    int ComputeValue(const std::vector<int> &var_values) const;
    void ComputeTableOfValues(std::vector<int> &computed_table_of_values) const;

    int CountVariables() const;
    int CountComplexity() const;
    void Print() const;
};

struct Poly {
    int64_t mincode;
    int64_t basic_complexity;
    std::vector<int> table_of_values;
    std::vector<int> poly_coeff;
    std::vector<Monom> polynom;

    void ConvertTableToPolynomCoeff();

    Poly() {}
    Poly(const BasicCircuit &basic);
    std::vector<int> GetTableOfValues() const {
        return table_of_values;
    }

    bool IsNullPolynom() const {
        return (polynom.size() == 0);
    }
    bool IsOnePolynom() const;
    int ComputeValue(const std::vector<int> &var_values) const;
    void ComputeTableOfValues(std::vector<int> &computed_table_of_values) const;
    int CountComplexity() const;
    void Print() const;
};

#endif