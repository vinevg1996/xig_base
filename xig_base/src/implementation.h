#ifndef IMPLEMENTATION
#define IMPLEMENTATION

#include "function.h"

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <memory>
#include <map>

struct IndexAndNegFlag {
    int index;
    bool neg_flag;

    IndexAndNegFlag() {}
    IndexAndNegFlag(int index, bool neg_flag) 
            : index(index)
            , neg_flag(neg_flag) {}
};

struct Precessors {
    int first_prec;
    int gate_and_first_prec_negation;
    int second_prec;
    int gate_and_second_prec_negation;
    //bool is_negative_vertex_flag;
    //bool variable_flag;
    std::vector<int> table_of_values;
    //Function func_ptr;

public:
    Precessors(int first_prec,
               int gate_and_first_prec_negation,
               int second_prec,
               int gate_and_second_prec_negation,
               const std::vector<int> &table_of_values);

    void PrintPrecessors() const;
};

struct ImplementationStruct {
    std::vector<std::shared_ptr<Function>> equiv_functions;
    std::vector<int> complexity_func;
    bool is_use_this_struct;
    int minimal_complexity;
    //std::vector<int> derive_complexity;

    ImplementationStruct() {}
};

class Circuit {
    std::shared_ptr<Function> func;
    std::map<std::vector<int>, ImplementationStruct> implement_map;
    std::vector<Precessors> circuit_matrix;

    bool IsFuncInImplementMap(const std::shared_ptr<Function> &func) const;
    bool IsMapImplFlagTrue(const std::shared_ptr<Function> &func) const;
    void AddToMap(const std::shared_ptr<Function> &func, int complexity);
    void AddToMapImplFlag(const std::shared_ptr<Function> &func, int new_complexity);
    int GetMinimalComplexity(const std::shared_ptr<Function> &func) const;
    int CountFuncComplexity(const std::shared_ptr<Function> &func);
    void CountPolyComplexity(const std::shared_ptr<Function> &curr_func);

    bool IsFuncInCircuitMatrix(const std::vector<int> &func_table_of_values) const;
    int GetIndexInCircuitMatrix(const std::vector<int> &func_table_of_values) const;
    int AddMonomWithTwoVarInCircuitMatrix(const Monom &curr_monom);
    int AddMonomWithThreeVarInCircuitMatrix(const Monom &curr_monom);
    int AddMonomWithBigVarInCircuitMatrix(const Monom &curr_monom);
    int WriteMonomToCircuitMatrix(const Monom &monom);
    std::vector<int> BuildXorFunction(const std::vector<int> &table1, const std::vector<int> &table2) const;
    std::vector<int> BuildMultFunction(const std::vector<int> &table1, const std::vector<int> &table2) const;
    std::vector<int> BuildNotFunction(const std::vector<int> &table1) const;

public:
    Circuit(const std::shared_ptr<Function> &func);
    int CountImplComplexity();
    void PrintMap() const;
    int Implement(const std::shared_ptr<Function> &func);
    //int ImplementWithWrite(const std::shared_ptr<Function> &func);
    IndexAndNegFlag WriteToCircuitMatrix(const std::shared_ptr<Function> &func);
    IndexAndNegFlag WritePolynomToCircuitMatrix(const std::shared_ptr<Function> &func);
    std::shared_ptr<Function> GetFunction() const {
        return func;
    }
    std::vector<Precessors> GetCircuitMatrix() const {
        return circuit_matrix;
    }
    std::vector<int> GetFinalStringTableOfValues() const;

    void PrintCircuitMatrix() const;
};

#endif