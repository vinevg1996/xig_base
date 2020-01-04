#ifndef FUNCTION
#define FUNCTION

#include "basic_circuit.h"
#include "polynom.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>

class Function {
    Poly polynom;
    bool is_polynom;
    std::shared_ptr<Function> func_with_var;
    std::shared_ptr<Function> func_without_var;
    std::shared_ptr<Monom> connect_monom;
    std::vector<int> table_of_values;

    void DividePolynom(Poly &poly_with_var, Poly &poly_without_var, const std::vector<int> &vec_for_connect_monom, int max_var_index);

public:
    Function() {}
    Function(const Function &func);
    Function(const Poly &polynom);
    void FindMaxVariable(int &max_index, int &max_value) const;
    bool IsPoly() const {
        return is_polynom;
    }
    bool IsPossibleToReduce() const;
    void Reduce();
    void ComputeTableOfValues(std::vector<int> &computed_table_of_values) const;
    bool IsMonomMultFunc() const;

    std::shared_ptr<Function> GetFuncWithVar() const {
        return func_with_var;
    }
    std::shared_ptr<Function> GetFuncWithoutVar() const {
        return func_without_var;
    }
    std::shared_ptr<Monom> GetConnectMonom() const {
        return connect_monom;
    }
    Poly GetPolynom() const {
        return polynom;
    }
    std::shared_ptr<Function> GetFuncMultMonom(const std::shared_ptr<Function> &func) const;
    void Print() const;
};

#endif