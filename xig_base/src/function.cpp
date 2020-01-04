#include "function.h"

#include <cstdlib>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <set>
#include <memory>

Function::Function(const Poly &polynom) 
        : polynom(polynom)
        , is_polynom(true)
        , table_of_values(polynom.table_of_values) 
        , func_with_var(nullptr)
        , func_without_var(nullptr)
        , connect_monom(nullptr) {}

Function::Function(const Function &func)
        : polynom(func.polynom)
        , is_polynom(func.is_polynom)
        , table_of_values(func.table_of_values) 
        , func_with_var(func.func_with_var)
        , func_without_var(func.func_without_var)
        , connect_monom(func.connect_monom) {}

void Function::FindMaxVariable(int &max_index, int &max_value) const {
    if (this->IsPoly()) {
        std::vector<int> max_variables = {0, 0, 0, 0, 0};
        for (const auto &curr_monom: polynom.polynom) {
            for (int i = 0; i < curr_monom.monom.size(); ++i) {
                if (curr_monom.monom[i]) {
                    ++max_variables[i];
                }
            }
        }
        auto result = std::max_element(max_variables.begin(), max_variables.end());
        max_index = std::distance(max_variables.begin(), result);
        max_value = max_variables[max_index];
        return;
    }
    max_index = -1;
    max_value = -1;
    return;
}

void Function::DividePolynom(Poly &poly_with_var, Poly &poly_without_var, const std::vector<int> &vec_for_connect_monom, int max_var_index) {
    for (const auto &curr_monom: polynom.polynom) {
        if (curr_monom.monom[max_var_index]) {
            Monom new_monom(curr_monom.monom);
            new_monom.monom[max_var_index] = 0;
            poly_with_var.polynom.push_back(new_monom);
        } else {
            poly_without_var.polynom.push_back(curr_monom);
        }
    }
}

void Function::Reduce() {
    if (this->IsPoly()) {
        int max_var_index, max_var_value;
        this->FindMaxVariable(max_var_index, max_var_value);
        if (max_var_value > 1) {
            Poly poly_with_var, poly_without_var;
            std::vector<int> vec_for_connect_monom(5, 0);
            vec_for_connect_monom[max_var_index] = 1;
            DividePolynom(poly_with_var, poly_without_var, vec_for_connect_monom, max_var_index);
            this->connect_monom = std::make_shared<Monom>(vec_for_connect_monom);
            this->func_with_var = std::make_shared<Function>(poly_with_var);
            this->func_without_var = std::make_shared<Function>(poly_without_var);

            this->is_polynom = false;
            this->func_with_var->Reduce();
            this->func_without_var->Reduce();
            return;
        } else {
            this->is_polynom = true;
        }
    }
    return;
}

void Function::ComputeTableOfValues(std::vector<int> &computed_table_of_values) const {
    if (this->IsPoly()) {
        this->polynom.ComputeTableOfValues(computed_table_of_values);
        return;
    } else {
        int size = 32;
        computed_table_of_values.resize(size, 0);
        std::vector<int> values_func_without_var(size, 0);
        std::vector<int> values_connect_monom(size, 0);
        std::vector<int> values_func_with_var(size, 0);
        if (this->func_without_var) {
            this->func_without_var->ComputeTableOfValues(values_func_without_var);
        }
        
        this->connect_monom->ComputeTableOfValues(values_connect_monom);
        this->func_with_var->ComputeTableOfValues(values_func_with_var);
        for (int i = 0; i < size; ++i) {
            computed_table_of_values[i] = (values_func_without_var[i] + values_connect_monom[i] * values_func_with_var[i]) % 2;
        }
        /*
        std::cout << "values_func_without_var:" << std::endl;
        for (int i = 0; i < values_func_without_var.size(); ++i) {
            std::cout << values_func_without_var[i] << " ";
        }
        std::cout << std::endl << "values_connect_monom:" << std::endl;
        for (int i = 0; i < values_connect_monom.size(); ++i) {
            std::cout << values_connect_monom[i] << " ";
        }
        std::cout << std::endl << "values_func_with_var:" << std::endl;
        for (int i = 0; i < values_func_with_var.size(); ++i) {
            std::cout << values_func_with_var[i] << " ";
        }
        std::cout << std::endl << "computed_table_of_values:" << std::endl;
        for (int i = 0; i < computed_table_of_values.size(); ++i) {
            std::cout << computed_table_of_values[i] << " ";
        }
        std::cout << std::endl;
        */
        return;
    }
}

bool Function::IsMonomMultFunc() const {
    if (!this->IsPoly()) {
        if (!this->func_without_var) {
            return true;
        } else {
            if (this->func_without_var->IsPoly()) {
                Poly func_without_var_poly = this->func_without_var->GetPolynom();
                return (func_without_var_poly.IsNullPolynom() ||
                        func_without_var_poly.IsOnePolynom());
            }
        }
        return false;
    }
    return false;
}

std::shared_ptr<Function> Function::GetFuncMultMonom(const std::shared_ptr<Function> &func) const {
    if (!func->IsMonomMultFunc()) {
        Function new_func(*func);
        new_func.func_without_var.reset();
        new_func.func_without_var = nullptr;
        std::vector<int> computed_table_of_values;
        new_func.ComputeTableOfValues(computed_table_of_values);
        new_func.table_of_values = computed_table_of_values;
        return std::make_shared<Function>(new_func);
    } else {
        return std::make_shared<Function>(*func);
    }
}

void Function::Print() const {
    if (this->IsPoly()) {
        this->polynom.Print();
    } else {
        if (func_without_var) {
            func_without_var->Print();
            std::cout << " + ";
        }
        connect_monom->Print();
        std::cout << "(";
        func_with_var->Print();
        std::cout << ")";
    }
}
