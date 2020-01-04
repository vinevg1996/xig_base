#include <iostream>
#include <cmath>
#include <map>

#include "basic_circuit.h"
#include "polynom.h"
#include "function.h"
#include "implementation.h"
#include "validator.h"

/*
int main(int argc, char **argv) {
    //std::ifstream data_base_file("../database_dump_AIG.txt");
    std::ifstream data_base_file("../example.txt");
    std::vector<Poly> poly_vector;
    //for (int i = 0; i < 616125; ++i) {
    std::vector<int> basic_table_of_values;
    for (int i = 0; i < 1; ++i) {
        BasicCircuit curr_basic_circuit(data_base_file);
        Poly curr_poly(curr_basic_circuit);
        poly_vector.push_back(curr_poly);
        basic_table_of_values = curr_basic_circuit.GetTableOfValues();
    }
    for (const auto &curr_poly: poly_vector) {
        std::cout << "Polynom:" << std::endl;
        curr_poly.Print();
        std::cout << std::endl << std::endl;
    }
    Function func(poly_vector[0]);
    func.Reduce();
    func.Print();
    std::cout << std::endl;
    Circuit(std::make_shared<Function>(func));
    return 0;
}
*/

void AddToMap(std::map<int, int> &success_map, int diff) {
    auto it = success_map.find(diff);
    if (it != success_map.end()) {
        ++(it->second);
    } else {
        success_map[diff] = 1;
    }
    return;
}

void PrintMap(const std::map<int, int> &success_map) {
    for (const auto &it: success_map) {
        std::cout << "(" << it.first << ": " << it.second << ")" << std::endl;
    }
}
/*
int main(int argc, char **argv) {
    std::ifstream data_base_file("../database_dump_AIG.txt");
    int success_count = 0;
    std::map<int, int> success_map;
    //for (int i = 0; i < 616125; ++i) {
    int i = 0;
    for (i = 0; i < 549000; ++i) {
        BasicCircuit curr_basic_circuit(data_base_file);
    }
    
    for (i = 549000; i < 550000; ++i) {
        int diff = 0;
        BasicCircuit curr_basic_circuit(data_base_file);
        int old_comlexity = curr_basic_circuit.GetComplexity();
        Poly curr_poly(curr_basic_circuit);
        Function func(curr_poly);
        func.Reduce();
        Circuit curr_circuit(std::make_shared<Function>(func));
        int new_comlexity = curr_circuit.CountImplComplexity();
        if (new_comlexity < old_comlexity) {
            diff = old_comlexity - new_comlexity;
            AddToMap(success_map, diff);
        }
        //std::cout << "old_comlexity = " << old_comlexity << std::endl;
        //std::cout << "new_comlexity = " << new_comlexity << std::endl;
        if (diff == 8) {
            std::cout << "i = " << i << ": " << std::endl;
            PrintMap(success_map);
        }
    }
    PrintMap(success_map);
    //std::cout << "success_count = " << success_count << std::endl;
    return 0;
}
*/

int main(int argc, char **argv) {
    
    //std::ifstream data_base_file("../database_dump_AIG.txt");
    std::ifstream data_base_file("../example.txt");
    std::vector<Poly> poly_vector;
    //for (int i = 0; i < 616125; ++i) {
    std::vector<int> basic_table_of_values;
    for (int i = 0; i < 1; ++i) {
        BasicCircuit curr_basic_circuit(data_base_file);
        Poly curr_poly(curr_basic_circuit);
        poly_vector.push_back(curr_poly);
        basic_table_of_values = curr_basic_circuit.GetTableOfValues();
    }
    
    for (const auto &curr_poly: poly_vector) {
        std::cout << "Polynom:" << std::endl;
        curr_poly.Print();
        std::cout << std::endl << std::endl;
    }
    
    Function func(poly_vector[0]);
    func.Reduce();
    func.Print();
    std::cout << std::endl << std::endl;
    std::shared_ptr<Function> test_func_ptr = std::make_shared<Function>(func);
    Circuit curr_circuit(test_func_ptr);
    IndexAndNegFlag id_and_neg_flag;
    id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(test_func_ptr);
    curr_circuit.PrintCircuitMatrix();
    std::cout << "index = " << id_and_neg_flag.index << std::endl;
    std::cout << "neg_flag = " << id_and_neg_flag.neg_flag << std::endl;
    std::cout << std::endl;
    Validator val(curr_circuit.GetFunction(), curr_circuit.GetCircuitMatrix());
    std::cout << "ValidateFlag:" << std::endl;
    std::cout << val.Validate() << std::endl;
    std::cout << "CircuitMatrix:" << std::endl;
    val.PrintCircuitMatrixValue();
    std::vector<int> func_value;
    test_func_ptr->ComputeTableOfValues(func_value);
    for (const auto &elem: func_value) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
    /*
    // 0 + x_2(1 + x_5) + x_1(1 + x_2 + x_5) + x_4(1 + x_5 + x_2(1 + x_5) + x_1(1 + x_2 + x_5)): 9
    // x_2 + x_2x_5 + x_1 + x_1x_2 + x_1x_5 + x_4 + x_4x_5 + x_2x_4 + x_2x_4x_5 + x_1x_4 + x_1x_2x_4 + x_1x_4x_5
    std::vector<int> test_vec1 = {0, 1, 0, 0, 0};
    std::vector<int> test_vec2 = {0, 1, 0, 0, 1};
    std::vector<int> test_vec3 = {1, 0, 0, 0, 0};
    std::vector<int> test_vec4 = {1, 1, 0, 0, 0};
    std::vector<int> test_vec5 = {1, 0, 0, 0, 1};
    std::vector<int> test_vec6 = {0, 0, 0, 1, 0};
    std::vector<int> test_vec7 = {0, 0, 0, 1, 1};
    std::vector<int> test_vec8 = {0, 1, 0, 1, 0};
    std::vector<int> test_vec9 = {0, 1, 0, 1, 1};
    std::vector<int> test_vec10 = {1, 0, 0, 1, 0};
    std::vector<int> test_vec11 = {1, 1, 0, 1, 0};
    std::vector<int> test_vec12 = {1, 0, 0, 1, 1};
    Monom test_monom1(test_vec1);
    Monom test_monom2(test_vec2);
    Monom test_monom3(test_vec3);
    Monom test_monom4(test_vec4);
    Monom test_monom5(test_vec5);
    Monom test_monom6(test_vec6);
    Monom test_monom7(test_vec7);
    Monom test_monom8(test_vec8);
    Monom test_monom9(test_vec9);
    Monom test_monom10(test_vec10);
    Monom test_monom11(test_vec11);
    Monom test_monom12(test_vec12);
    Poly test_poly;
    test_poly.polynom.push_back(test_monom1);
    test_poly.polynom.push_back(test_monom2);
    test_poly.polynom.push_back(test_monom3);
    test_poly.polynom.push_back(test_monom4);
    test_poly.polynom.push_back(test_monom5);
    test_poly.polynom.push_back(test_monom6);
    test_poly.polynom.push_back(test_monom7);
    test_poly.polynom.push_back(test_monom8);
    test_poly.polynom.push_back(test_monom9);
    test_poly.polynom.push_back(test_monom10);
    test_poly.polynom.push_back(test_monom11);
    test_poly.polynom.push_back(test_monom12);
    std::vector<int> computed_table_of_values;
    test_poly.ComputeTableOfValues(computed_table_of_values);
    test_poly.table_of_values = computed_table_of_values;
    
    Function test_func(test_poly);
    test_func.Print();
    std::cout << std::endl;
    std::cout << std::endl;
    test_func.Reduce();
    test_func.Print();
    std::cout << std::endl << "_____________________" << std::endl;
    std::cout << std::endl;
    std::shared_ptr<Function> test_func_ptr = std::make_shared<Function>(test_func);
    Circuit curr_circuit(test_func_ptr);    
    IndexAndNegFlag id_and_neg_flag;
    id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(test_func_ptr);
    curr_circuit.PrintCircuitMatrix();
    std::cout << "index = " << id_and_neg_flag.index << std::endl;
    std::cout << "neg_flag = " << id_and_neg_flag.neg_flag << std::endl;
    */
    return 0;
}

/*
int main(int argc, char **argv) {
    
    // 0 + x_4(1 + x_1) + x_5(1 + x_1 + x_2(1 + x_4)): 9 (5)
    std::vector<int> test_vec1 = {1, 0, 0, 0, 0};
    std::vector<int> test_vec2 = {1, 0, 0, 1, 0};
    std::vector<int> test_vec3 = {0, 0, 0, 0, 1};
    std::vector<int> test_vec4 = {1, 0, 0, 0, 1};
    std::vector<int> test_vec5 = {0, 1, 0, 0, 1};
    std::vector<int> test_vec6 = {0, 1, 0, 1, 1};
    Monom test_monom1(test_vec1);
    Monom test_monom2(test_vec2);
    Monom test_monom3(test_vec3);
    Monom test_monom4(test_vec4);
    Monom test_monom5(test_vec5);
    Monom test_monom6(test_vec6);
    Poly test_poly;
    test_poly.polynom.push_back(test_monom1);
    test_poly.polynom.push_back(test_monom2);
    test_poly.polynom.push_back(test_monom3);
    test_poly.polynom.push_back(test_monom4);
    test_poly.polynom.push_back(test_monom5);
    test_poly.polynom.push_back(test_monom6);
    std::vector<int> computed_table_of_values;
    test_poly.ComputeTableOfValues(computed_table_of_values);
    test_poly.table_of_values = computed_table_of_values;
    
    Function test_func(test_poly);
    test_func.Print();
    std::cout << std::endl << std::endl;
    test_func.Reduce();
    test_func.Print();
    std::cout << std::endl;
    std::shared_ptr<Function> test_func_ptr = std::make_shared<Function>(test_func);
    Circuit curr_circuit(test_func_ptr);
    IndexAndNegFlag id_and_neg_flag;
    id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(test_func_ptr);
    curr_circuit.PrintCircuitMatrix();
    std::cout << "index = " << id_and_neg_flag.index << std::endl;
    std::cout << "neg_flag = " << id_and_neg_flag.neg_flag << std::endl;
    return 0;
    
    
    std::vector<int> test_vec0 = {0, 0, 0, 0, 0};
    //std::vector<int> test_vec1 = {0, 0, 0, 0, 1};
    std::vector<int> test_vec1 = {0, 0, 1, 1, 0};
    std::vector<int> test_vec2 = {0, 1, 1, 1, 0};
    //std::vector<int> test_vec2 = {0, 0, 0, 1, 0};
    Monom test_monom0(test_vec0);
    Monom test_monom1(test_vec1);
    Monom test_monom2(test_vec2);
    Poly test_poly;
    test_poly.polynom.push_back(test_monom0);
    test_poly.polynom.push_back(test_monom1);
    test_poly.polynom.push_back(test_monom2);
    Function test_func(test_poly);
    test_func.Print();
    std::cout << std::endl;
    std::shared_ptr<Function> test_func_ptr = std::make_shared<Function>(test_func);
    Circuit curr_circuit(test_func_ptr);
    IndexAndNegFlag id_and_neg_flag;
    id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(test_func_ptr);
    curr_circuit.PrintCircuitMatrix();
    std::cout << "index = " << id_and_neg_flag.index << std::endl;
    std::cout << "neg_flag = " << id_and_neg_flag.neg_flag << std::endl;
    return 0;
    
}
*/
