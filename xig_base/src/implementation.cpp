#include "implementation.h"

#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <set>
#include <memory>

// Precessors
Precessors::Precessors(int first_prec,
                       int gate_and_first_prec_negation,
                       int second_prec,
                       int gate_and_second_prec_negation,
                       const std::vector<int> &table_of_values)
       : first_prec(first_prec)
       , gate_and_first_prec_negation(gate_and_first_prec_negation)
       , second_prec(second_prec)
       , gate_and_second_prec_negation(gate_and_second_prec_negation) 
       , table_of_values(table_of_values) {}

void Precessors::PrintPrecessors() const {
    std::cout << first_prec << ' ' << gate_and_first_prec_negation
    << ' ' << second_prec << ' ' << gate_and_second_prec_negation
    << std::endl;
}

// Circuit
Circuit::Circuit(const std::shared_ptr<Function> &func)
        : func(func) {
    for (int i = 0; i < 5; ++i) {
        std::vector<int> monom_vec(5, 0);
        monom_vec[i] = 1;
        std::vector<int> table_of_values;
        Monom monom(monom_vec);
        monom.ComputeTableOfValues(table_of_values);
        Poly poly;
        poly.polynom.push_back(monom);
        Function curr_func(poly);
        Precessors curr_prec(0, 0, 0, 0, table_of_values);
        circuit_matrix.push_back(curr_prec);
    }
}

int Circuit::CountImplComplexity() {
    this->CountPolyComplexity(func);
    int complexity = this->CountFuncComplexity(func);
    //std::cout << "complexity = " << complexity << std::endl;
    //std::cout << "___________________________" << std::endl;
    //complexity = this->ImplementWithWrite(func);
    complexity = this->Implement(func);
    //std::cout << "complexity = " << complexity << std::endl;
    return complexity;
}

bool Circuit::IsFuncInImplementMap(const std::shared_ptr<Function> &func) const {
    std::vector<int> func_table_of_values;
    func->ComputeTableOfValues(func_table_of_values);
    auto it = implement_map.find(func_table_of_values);
    return (it != implement_map.end());
}

void Circuit::AddToMap(const std::shared_ptr<Function> &func, int complexity) {
    if (!IsFuncInImplementMap(func)) {
        std::vector<int> func_table_of_values;
        func->ComputeTableOfValues(func_table_of_values);
        ImplementationStruct impl_sturct;
        impl_sturct.equiv_functions.push_back(func);
        impl_sturct.complexity_func.push_back(complexity);
        auto it_begin = impl_sturct.complexity_func.begin();
        auto it_end = impl_sturct.complexity_func.end();
        auto it_min = std::min(it_begin, it_end);
        impl_sturct.minimal_complexity = *it_min;
        //impl_sturct.is_use_this_struct = true;
        impl_sturct.is_use_this_struct = false;
        implement_map[func_table_of_values] = impl_sturct;
        /*
        std::cout << "___________________________" << std::endl;
        func->Print();
        std::cout << std::endl << "Current Map" << std::endl;
        this->PrintMap();
        std::cout << "___________________________" << std::endl;
        */
    }
    return;
}

int Circuit::GetMinimalComplexity(const std::shared_ptr<Function> &func) const {
    std::vector<int> func_table_of_values;
    func->ComputeTableOfValues(func_table_of_values);
    auto it = implement_map.find(func_table_of_values);
    return it->second.minimal_complexity;
}

int Circuit::CountFuncComplexity(const std::shared_ptr<Function> &func) {
    if (func->IsPoly()) {
        std::vector<int> poly_table_of_values;
        func->ComputeTableOfValues(poly_table_of_values);
        auto it = implement_map.find(poly_table_of_values);
        return it->second.minimal_complexity;
    } else {
        if (IsFuncInImplementMap(func)) {
            return GetMinimalComplexity(func);
        } else {
            std::shared_ptr<Function> func_without_var = func->GetFuncWithoutVar();
            std::shared_ptr<Function> func_with_var = func->GetFuncWithVar();
            std::shared_ptr<Function> func_mult_monom = func->GetFuncMultMonom(func);
            int complexity1 = 0, complexity2 = 0;
            if (func->IsMonomMultFunc()) {
                if (IsFuncInImplementMap(func_mult_monom)) {
                    return GetMinimalComplexity(func_mult_monom);
                } else {
                    if (IsFuncInImplementMap(func_with_var)) {
                        complexity2 = GetMinimalComplexity(func_with_var);
                        AddToMap(func_mult_monom, complexity2 + 1);
                        return complexity2 + 1;
                    } else {
                        complexity2 = CountFuncComplexity(func_with_var);
                        AddToMap(func_with_var, complexity2);
                        AddToMap(func_mult_monom, complexity2 + 1);
                        return complexity2 + 1;
                    }
                }
            } else {
                if (IsFuncInImplementMap(func_without_var)) {
                    complexity1 = GetMinimalComplexity(func_without_var);
                } else {
                    complexity1 = CountFuncComplexity(func_without_var);
                    AddToMap(func_without_var, complexity1);
                }
                if (IsFuncInImplementMap(func_mult_monom)) {
                    complexity2 = GetMinimalComplexity(func_mult_monom);
                    return complexity1 + complexity2 + 1;
                } else {
                    if (IsFuncInImplementMap(func_with_var)) {
                        complexity2 = GetMinimalComplexity(func_with_var);
                        //AddToMap(func_with_var, complexity2 + 1);
                    } else {
                        complexity2 = CountFuncComplexity(func_with_var);
                        AddToMap(func_with_var, complexity2);
                    }
                    AddToMap(func_mult_monom, complexity2 + 1);
                    AddToMap(func, complexity1 + complexity2 + 2);
                    return complexity1 + complexity2 + 2;
                }
            }
        }
    }
}

void Circuit::AddToMapImplFlag(const std::shared_ptr<Function> &func, int new_complexity) {
    std::vector<int> func_table_of_values;
    func->ComputeTableOfValues(func_table_of_values);
    auto it = implement_map.find(func_table_of_values);
    it->second.is_use_this_struct = true;
    if (new_complexity < it->second.minimal_complexity) {
        //std::cout << "old_complexity = " << it->second.minimal_complexity << std::endl;
        //std::cout << "new_complexity = " << new_complexity << std::endl;
        it->second.minimal_complexity = new_complexity;
    }
    /*
    std::cout << "___________________________" << std::endl;
    func->Print();
    std::cout << std::endl << "Current Map" << std::endl;
    this->PrintMap();
    std::cout << "___________________________" << std::endl;
    */
    return;
}

bool Circuit::IsMapImplFlagTrue(const std::shared_ptr<Function> &func) const {
    std::vector<int> func_table_of_values;
    func->ComputeTableOfValues(func_table_of_values);
    auto it = implement_map.find(func_table_of_values);
    return it->second.is_use_this_struct;
}

bool Circuit::IsFuncInCircuitMatrix(const std::vector<int> &func_table_of_values) const {
    for (const auto &curr_prec: circuit_matrix) {
        if (curr_prec.table_of_values == func_table_of_values) {
            return true;
        }
    }
    return false;
}

int Circuit::GetIndexInCircuitMatrix(const std::vector<int> &func_table_of_values) const {
    for (int i = 0; i < circuit_matrix.size(); ++i) {
        if (circuit_matrix[i].table_of_values == func_table_of_values) {
            return i;
        }
    }
}

int Circuit::AddMonomWithTwoVarInCircuitMatrix(const Monom &curr_monom) {
    std::vector<int> prec_vertex;
    std::vector<int> curr_table_of_values;
    curr_monom.ComputeTableOfValues(curr_table_of_values);
    for (int i = 0; i < curr_monom.monom.size(); ++i) {
        if (curr_monom.monom[i]) {
            prec_vertex.push_back(i);
        }
    }
    Precessors new_prec(prec_vertex[0], 0, prec_vertex[1], 0, curr_table_of_values);
    circuit_matrix.push_back(new_prec);
    return (circuit_matrix.size() - 1);
}

int Circuit::AddMonomWithThreeVarInCircuitMatrix(const Monom &curr_monom) {
    std::vector<int> monom_table_of_values;
    curr_monom.ComputeTableOfValues(monom_table_of_values);
    std::vector<int> variables;
    for (int i = 0; i < curr_monom.monom.size(); ++i) {
        if (curr_monom.monom[i]) {
            variables.push_back(i);
        }
    }
    Monom monom1(curr_monom);
    monom1.monom[variables[2]] = 0;
    std::vector<int> monom1_table_of_values;
    monom1.ComputeTableOfValues(monom1_table_of_values);
    if (IsFuncInCircuitMatrix(monom1_table_of_values)) {
        int prec_vertex1 = GetIndexInCircuitMatrix(monom1_table_of_values);
        int prec_vertex2 = variables[2];
        Precessors new_prec(prec_vertex1, 0, prec_vertex2, 0, monom_table_of_values);
        circuit_matrix.push_back(new_prec);
        return (circuit_matrix.size() - 1);
    } else {
        Monom monom2(curr_monom);
        monom2.monom[variables[1]] = 0;
        std::vector<int> monom2_table_of_values;
        monom2.ComputeTableOfValues(monom2_table_of_values);
        if (IsFuncInCircuitMatrix(monom2_table_of_values)) {
            int prec_vertex1 = GetIndexInCircuitMatrix(monom2_table_of_values);
            int prec_vertex2 = variables[1];
            Precessors new_prec(prec_vertex1, 0, prec_vertex2, 0, monom_table_of_values);
            circuit_matrix.push_back(new_prec);
            return (circuit_matrix.size() - 1);
        } else {
            Monom monom3(curr_monom);
            monom3.monom[variables[0]] = 0;
            std::vector<int> monom3_table_of_values;
            monom3.ComputeTableOfValues(monom3_table_of_values);
            if (IsFuncInCircuitMatrix(monom3_table_of_values)) {
                int prec_vertex1 = GetIndexInCircuitMatrix(monom3_table_of_values);
                int prec_vertex2 = variables[0];
                Precessors new_prec(prec_vertex1, 0, prec_vertex2, 0, monom_table_of_values);
                circuit_matrix.push_back(new_prec);
                return (circuit_matrix.size() - 1);
            }
        }
    }
    int prec_vertex1, prec_vertex2;
    prec_vertex1 = AddMonomWithTwoVarInCircuitMatrix(monom1);
    prec_vertex2 = variables[2];
    Precessors new_prec(prec_vertex1, 0, prec_vertex2, 0, monom_table_of_values);
    circuit_matrix.push_back(new_prec);
    return (circuit_matrix.size() - 1);
}

int Circuit::AddMonomWithBigVarInCircuitMatrix(const Monom &curr_monom) {
    std::vector<int> variables;
    for (int i = 0; i < curr_monom.monom.size(); ++i) {
        if (curr_monom.monom[i]) {
            variables.push_back(i);
        }
    }
    std::vector<int> monom_vec(5, 0);
    Monom new_monom(monom_vec);
    int prec_vertex = variables[0];
    new_monom.monom[prec_vertex] = 1;
    for (int i = 1; i < variables.size(); ++i) {
        new_monom.monom[variables[i]] = 1;
        std::vector<int> curr_table_of_values;
        new_monom.ComputeTableOfValues(curr_table_of_values);
        Precessors new_prec(prec_vertex, 0, variables[i], 0, curr_table_of_values);
        circuit_matrix.push_back(new_prec);
        prec_vertex = circuit_matrix.size() - 1;
    }
    return (circuit_matrix.size() - 1);
}

int Circuit::WriteMonomToCircuitMatrix(const Monom &monom) {
    if (monom.IsVariable()) {
        return monom.GetSingleVariable();
    }
    //int var_number = monom.CountVariables();
    std::vector<int> monom_variables;
    for (int i = 0; i < 5; ++i) {
        if (monom.monom[i]) {
            monom_variables.push_back(i);
        }
    }
    std::vector<int> monom_table_of_values;
    monom.ComputeTableOfValues(monom_table_of_values);
    if (IsFuncInCircuitMatrix(monom_table_of_values)) {
        return GetIndexInCircuitMatrix(monom_table_of_values);
    }
    int prec_vertex;
    if (monom_variables.size() == 2) {
        prec_vertex = AddMonomWithTwoVarInCircuitMatrix(monom);
        return prec_vertex;
    }
    if (monom_variables.size() == 3) {
        prec_vertex = AddMonomWithThreeVarInCircuitMatrix(monom);
        return prec_vertex;
    }
    prec_vertex = AddMonomWithBigVarInCircuitMatrix(monom);
    return prec_vertex;
}

std::vector<int> Circuit::BuildXorFunction(const std::vector<int> &table1, const std::vector<int> &table2) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] + table2[i]) % 2;
    }
    return new_table_of_values;
}

std::vector<int> Circuit::BuildMultFunction(const std::vector<int> &table1, const std::vector<int> &table2) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] * table2[i]) % 2;
    }
    return new_table_of_values;
}

std::vector<int> Circuit::BuildNotFunction(const std::vector<int> &table1) const {
    std::vector<int> new_table_of_values(table1.size(), 0);
    for (int i = 0; i < table1.size(); ++i) {
        new_table_of_values[i] = (table1[i] + 1) % 2;
    }
    return new_table_of_values;
}

IndexAndNegFlag Circuit::WritePolynomToCircuitMatrix(const std::shared_ptr<Function> &func) {
    int index = -1;
    bool neg_flag = false;
    int i = 0;
    Poly curr_poly = func->GetPolynom();
    std::vector<int> poly_table_of_values;
    curr_poly.ComputeTableOfValues(poly_table_of_values);
    
    if (curr_poly.polynom[0].IsNullMonom()) {
        neg_flag = true;
        ++i;
        std::vector<int> neg_poly_table_of_values;
        neg_poly_table_of_values = BuildNotFunction(poly_table_of_values);
        if (IsFuncInCircuitMatrix(neg_poly_table_of_values)) {
            index = GetIndexInCircuitMatrix(neg_poly_table_of_values);
            return IndexAndNegFlag(index, true);
        }
    } else {
        if(IsFuncInCircuitMatrix(poly_table_of_values)) {
            index = GetIndexInCircuitMatrix(poly_table_of_values);
            return IndexAndNegFlag(index, false);
        }
    }
    std::vector<int> prec_table_of_values;
    curr_poly.polynom[i].ComputeTableOfValues(prec_table_of_values);
    if (IsFuncInCircuitMatrix(prec_table_of_values)) {
        index = GetIndexInCircuitMatrix(prec_table_of_values);
    } else {
        index = WriteMonomToCircuitMatrix(curr_poly.polynom[i]);
    }
    if (i == curr_poly.polynom.size() - 1) {
        // polynom is monom
        return IndexAndNegFlag(index, neg_flag);
    }
    ++i;
    int index2;
    for ( ; i < curr_poly.polynom.size(); ++i) {
        std::vector<int> monom_table_of_values;
        std::vector<int> new_table_of_values;
        curr_poly.polynom[i].ComputeTableOfValues(monom_table_of_values);
        new_table_of_values = BuildXorFunction(prec_table_of_values, monom_table_of_values);
        if (IsFuncInCircuitMatrix(new_table_of_values)) {
            index = GetIndexInCircuitMatrix(new_table_of_values);
        } else {
            if (IsFuncInCircuitMatrix(monom_table_of_values)) {
                index2 = GetIndexInCircuitMatrix(monom_table_of_values);
            } else {
                index2 = WriteMonomToCircuitMatrix(curr_poly.polynom[i]);
            }
            Precessors new_prec(index, 2, index2, 2, new_table_of_values);
            circuit_matrix.push_back(new_prec);
            index = circuit_matrix.size() - 1;
        }
        prec_table_of_values = new_table_of_values;
    }
    return IndexAndNegFlag(index, neg_flag);
}

/*
IndexAndNegFlag Circuit::WritePolynomToCircuitMatrix(const std::shared_ptr<Function> &func) {
    int index = 0;
    bool neg_flag = false;
    Poly curr_poly = func->GetPolynom();
    std::vector<int> poly_table_of_values;
    curr_poly.ComputeTableOfValues(poly_table_of_values);
    if(IsFuncInCircuitMatrix(poly_table_of_values)) {
        index = GetIndexInCircuitMatrix(poly_table_of_values);
        return IndexAndNegFlag(index, false);
    } else {
        int i = 0;
        if (curr_poly.polynom[0].IsNullMonom()) {
            neg_flag = true;
            ++i;
        }
        std::vector<int> prec_table_of_values;
        curr_poly.polynom[i].ComputeTableOfValues(prec_table_of_values);
        if (IsFuncInCircuitMatrix(prec_table_of_values)) {
            index = GetIndexInCircuitMatrix(prec_table_of_values);
        } else {
            index = WriteMonomToCircuitMatrix(curr_poly.polynom[i]);
        }
        if (i == curr_poly.polynom.size() - 1) {
            // polynom is monom
            return IndexAndNegFlag(index, neg_flag);
        }
        ++i;
        int index2;
        for ( ; i < curr_poly.polynom.size(); ++i) {
            std::vector<int> monom_table_of_values;
            std::vector<int> new_table_of_values;
            curr_poly.polynom[i].ComputeTableOfValues(monom_table_of_values);
            new_table_of_values = BuildXorFunction(prec_table_of_values, monom_table_of_values);
            if (IsFuncInCircuitMatrix(new_table_of_values)) {
                index = GetIndexInCircuitMatrix(new_table_of_values);
            } else {
                if (IsFuncInCircuitMatrix(monom_table_of_values)) {
                    index2 = GetIndexInCircuitMatrix(monom_table_of_values);
                } else {
                    index2 = WriteMonomToCircuitMatrix(curr_poly.polynom[i]);
                }
                Precessors new_prec(index, 2, index2, 2, new_table_of_values);
                circuit_matrix.push_back(new_prec);
                index = circuit_matrix.size() - 1;
            }
            prec_table_of_values = new_table_of_values;
        }
    }
    return IndexAndNegFlag(index, neg_flag);
}
*/

// only if all subfunctions is implemented
// return index in circuit_matrix
IndexAndNegFlag Circuit::WriteToCircuitMatrix(const std::shared_ptr<Function> &func) {
    std::shared_ptr<Function> func_without_var = func->GetFuncWithoutVar();
    std::shared_ptr<Function> func_with_var = func->GetFuncWithVar();
    std::shared_ptr<Function> func_mult_monom = func->GetFuncMultMonom(func);
    std::shared_ptr<Monom> connect_monom = func->GetConnectMonom();

    int index = -1;
    std::vector<int> value_func;
    func->ComputeTableOfValues(value_func);
    if ((func_without_var) && ((func_without_var->GetPolynom()).IsOnePolynom())) {
        std::vector<int> not_value_func;
        not_value_func = BuildNotFunction(value_func);
        if (IsFuncInCircuitMatrix(not_value_func)) {
            index = GetIndexInCircuitMatrix(not_value_func);
            return IndexAndNegFlag(index, true);
        }
    } else {
        if (IsFuncInCircuitMatrix(value_func)) {
            index = GetIndexInCircuitMatrix(value_func);
            return IndexAndNegFlag(index, false);
        }
    }
    if (func->IsPoly()) {
        return WritePolynomToCircuitMatrix(func);
    }
    bool neg_flag = false;
    std::vector<int> value_func_without_var;
    std::vector<int> value_func_with_var;
    std::vector<int> value_func_mult_monom;
    std::vector<int> value_connect_monom;
    IndexAndNegFlag id_and_neg_flag1, id_and_neg_flag2;
    if (func->IsMonomMultFunc()) {
        // x * f2
        id_and_neg_flag1 = WriteToCircuitMatrix(func_with_var);
        connect_monom->ComputeTableOfValues(value_connect_monom);
        func_mult_monom->ComputeTableOfValues(value_func_mult_monom);
        int index2 = GetIndexInCircuitMatrix(value_connect_monom);
        if (id_and_neg_flag1.neg_flag) {
            Precessors new_prec(index2, 0, id_and_neg_flag1.index, 1, value_func_mult_monom);
            circuit_matrix.push_back(new_prec);
        } else {
            Precessors new_prec(index2, 0, id_and_neg_flag1.index, 0, value_func_mult_monom);
            circuit_matrix.push_back(new_prec);
        }
        /*
        if (func_without_var) {
            if (func_without_var->IsPoly()) {
                Poly curr_poly = func_without_var->GetPolynom();
                if (curr_poly.IsOnePolynom()) {
                    return IndexAndNegFlag(circuit_matrix.size() - 1, true);
                }
            }
        }
        */
        if ((func_without_var) && (func_without_var->IsPoly())) {
            Poly curr_poly = func_without_var->GetPolynom();
            if (curr_poly.IsOnePolynom()) {
                return IndexAndNegFlag(circuit_matrix.size() - 1, true);
            }
        }
        return IndexAndNegFlag(circuit_matrix.size() - 1, false);
    } else {
        // f1 + x * f2
        int prec_neg1, prec_neg2;
        id_and_neg_flag1 = WriteToCircuitMatrix(func_without_var);
        id_and_neg_flag2 = WriteToCircuitMatrix(func_mult_monom);
        if (id_and_neg_flag1.neg_flag) {
            prec_neg1 = 3;
        } else {
            prec_neg1 = 2;
        }
        if (id_and_neg_flag2.neg_flag) {
            prec_neg2 = 3;
        } else {
            prec_neg2 = 2;
        }
        Precessors new_prec(id_and_neg_flag1.index, prec_neg1, id_and_neg_flag2.index, prec_neg2, value_func);
        circuit_matrix.push_back(new_prec);
        return IndexAndNegFlag(circuit_matrix.size() - 1, false);
    }
}
/*
int Circuit::ImplementWithWrite(const std::shared_ptr<Function> &func) {
    if (IsMapImplFlagTrue(func)) {
        return 0;
    }
    int new_complexity;
    if (func->IsPoly()) {
        new_complexity = GetMinimalComplexity(func);
        AddToMapImplFlag(func, new_complexity);
        return new_complexity;
    } else {
        std::shared_ptr<Function> func_without_var = func->GetFuncWithoutVar();
        std::shared_ptr<Function> func_with_var = func->GetFuncWithVar();
        std::shared_ptr<Function> func_mult_monom = func->GetFuncMultMonom(func);
        int new_complexity1 = 0, new_complexity2 = 0;
        if (func->IsMonomMultFunc()) {
            // x * f2
            new_complexity2 = Implement(func_with_var);
            AddToMapImplFlag(func_mult_monom, new_complexity2 + 1);
            return new_complexity2 + 1;
        } else {
            // f1 + x * f2
            new_complexity1 = Implement(func_without_var);
            if (IsMapImplFlagTrue(func_mult_monom)) {
                new_complexity2 = 0;
                AddToMapImplFlag(func, new_complexity1 + 1);
                return new_complexity1 + 1;
            } else {
                new_complexity2 = Implement(func_with_var);
                AddToMapImplFlag(func_mult_monom, new_complexity2 + 1);
                AddToMapImplFlag(func, new_complexity1 + new_complexity2 + 2);
                return new_complexity1 + new_complexity2 + 2;
            }
        }
    }
}
*/
int Circuit::Implement(const std::shared_ptr<Function> &func) {
    if (IsMapImplFlagTrue(func)) {
        return 0;
    }
    int new_complexity;
    if (func->IsPoly()) {
        new_complexity = GetMinimalComplexity(func);
        AddToMapImplFlag(func, new_complexity);
        return new_complexity;
    } else {
        std::shared_ptr<Function> func_without_var = func->GetFuncWithoutVar();
        std::shared_ptr<Function> func_with_var = func->GetFuncWithVar();
        std::shared_ptr<Function> func_mult_monom = func->GetFuncMultMonom(func);
        int new_complexity1 = 0, new_complexity2 = 0;
        if (func->IsMonomMultFunc()) {
            // x * f2
            new_complexity2 = Implement(func_with_var);
            AddToMapImplFlag(func_mult_monom, new_complexity2 + 1);
            return new_complexity2 + 1;
        } else {
            // f1 + x * f2
            new_complexity1 = Implement(func_without_var);
            if (IsMapImplFlagTrue(func_mult_monom)) {
                new_complexity2 = 0;
                AddToMapImplFlag(func, new_complexity1 + 1);
                return new_complexity1 + 1;
            } else {
                new_complexity2 = Implement(func_with_var);
                AddToMapImplFlag(func_mult_monom, new_complexity2 + 1);
                AddToMapImplFlag(func, new_complexity1 + new_complexity2 + 2);
                return new_complexity1 + new_complexity2 + 2;
            }
        }
    }
}

/*
int Circuit::CountFuncComplexity(const std::shared_ptr<Function> &func) {
    int complexity1 = 0;
    int complexity2 = 0;
    if (func->IsPoly()) {
        std::vector<int> poly_table_of_values;
        func->ComputeTableOfValues(poly_table_of_values);
        auto it = implement_map.find(poly_table_of_values);
        if (!it->second.is_use_this_struct) {
            //it->second.is_use_this_struct = true;
            return it->second.minimal_complexity;
        } else {
            return 0;
        }
    } else {
        if (IsFuncInImplementMap(func)) {
            return 0;
        } else {
            std::shared_ptr<Function> func_without_var = func->GetFuncWithoutVar();
            std::shared_ptr<Function> func_with_var = func->GetFuncWithVar();
            std::shared_ptr<Function> func_mult_monom = func->GetFuncMultMonom(func);
            int complexity1 = 0, complexity2 = 0;
            if (func->IsMonomMultFunc()) {
                if (IsFuncInImplementMap(func_mult_monom)) {
                    return 0;
                } else {
                    if (IsFuncInImplementMap(func_with_var)) {
                        AddToMap(func_mult_monom, 1);
                        return 1;
                    } else {
                        complexity2 = CountFuncComplexity(func_with_var);
                        AddToMap(func_with_var, complexity2);
                        AddToMap(func_mult_monom, complexity2 + 1);
                        return complexity2 + 1;
                    }
                }
            } else {
                if (IsFuncInImplementMap(func_without_var)) {
                    complexity1 = 0;
                } else {
                    complexity1 = CountFuncComplexity(func_without_var);
                    AddToMap(func_without_var, complexity1);
                }
                if (IsFuncInImplementMap(func_mult_monom)) {
                    complexity2 = 0;
                } else {
                    if (IsFuncInImplementMap(func_with_var)) {
                        AddToMap(func_mult_monom, 1);
                        complexity2 = 1;
                    } else {
                        complexity2 = CountFuncComplexity(func_with_var);
                        AddToMap(func_with_var, complexity2);
                        AddToMap(func_mult_monom, complexity2 + 1);
                        AddToMap(func, complexity1 + complexity2 + 1);
                    }
                    return complexity1 + complexity2 + 1;
                }
            }
        }
    }
}
*/

void Circuit::CountPolyComplexity(const std::shared_ptr<Function> &curr_func) {
    if(curr_func->IsPoly()) {
        std::vector<int> poly_table_of_values;
        curr_func->ComputeTableOfValues(poly_table_of_values);
        auto it = implement_map.find(poly_table_of_values);
        if (it == implement_map.end()) {
            ImplementationStruct impl_sturct;
            impl_sturct.equiv_functions.push_back(curr_func);
            int comp = curr_func->GetPolynom().CountComplexity();
            impl_sturct.complexity_func.push_back(comp);
            impl_sturct.minimal_complexity = comp;
            impl_sturct.is_use_this_struct = false;
            //mymap.insert ( std::pair<char,int>('z',500) );
            //implement_map.insert(std::pair<std::vector<int>, ImplementationStruct>(, ));
            implement_map[poly_table_of_values] = impl_sturct;
        }
    } else {
        this->CountPolyComplexity(curr_func->GetFuncWithVar());
        this->CountPolyComplexity(curr_func->GetFuncWithoutVar());
    }
}

void Circuit::PrintMap() const {
    for (const auto &it_map: implement_map) {
        for (const auto &elem: it_map.first) {
            std::cout << elem;
        }
        std::cout << ":" << std::endl;
        for (int i = 0; i < it_map.second.equiv_functions.size(); ++i) {
            std::cout << it_map.second.is_use_this_struct << "; ";
            it_map.second.equiv_functions[i]->Print();
            std::cout << ": " << it_map.second.minimal_complexity << std::endl;
        }
        //std::cout << std::endl;
    }
}

std::vector<int> Circuit::GetFinalStringTableOfValues() const {
    return circuit_matrix[circuit_matrix.size() - 1].table_of_values;
}

void Circuit::PrintCircuitMatrix() const {
    for (int i = 0; i < circuit_matrix.size(); ++i) {
        std::cout << "i = " << i << ": ";
        circuit_matrix[i].PrintPrecessors();
    }
    std::cout << std::endl;
}
