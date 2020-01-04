#include <iostream>
#include <cmath>
#include <string>
#include <map>

#include "basic_circuit.h"
#include "polynom.h"
#include "function.h"
#include "implementation.h"
#include "validator.h"

void AddToMap(std::map<int, int> &success_map, int diff) {
    auto it = success_map.find(diff);
    if (it != success_map.end()) {
        ++(it->second);
    } else {
        success_map[diff] = 1;
    }
    return;
}

void PrintMap(const std::map<int, int> &success_map, std::ofstream &out_file) {
    for (const auto &it: success_map) {
        out_file << "(" << it.first << ": " << it.second << ")" << std::endl;
    }
}

/*
// total_circuits = 616125
// argv[1] = number_of_process
int main(int argc, char const **argv) {
    std::ifstream data_base_file("example2.txt");
    int max_complexity = 0;
    int id_max_complexity = 0;
    std::string str1 = std::string("../output_files/data_base_xig");
    std::string str2 = std::string(argv[1]);
    std::string str3 = std::string(".txt");
    std::string out_file_name = str1 + str2 + str3;
    std::ofstream data_xig_base_file(out_file_name);
    int diff = 0;
    BasicCircuit curr_basic_circuit(data_base_file);
    int old_complexity = curr_basic_circuit.GetComplexity();
    Poly curr_poly(curr_basic_circuit);
    Function func(curr_poly);
    
    func.Print();
    std::cout << std::endl;
    
    std::vector<int> source_func_value;
    func.ComputeTableOfValues(source_func_value);
    std::cout << "source_func_value:" << std::endl;
    for (const auto &elem: source_func_value) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    func.Reduce();
    
    func.Print();
    std::cout << std::endl;
    
    std::shared_ptr<Function> func_ptr = std::make_shared<Function>(func);
    Circuit curr_circuit(func_ptr);
    IndexAndNegFlag id_and_neg_flag;
    id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(func_ptr);
    Validator val(curr_circuit.GetFunction(), 
                  curr_circuit.GetCircuitMatrix(),
                  id_and_neg_flag.index,
                  id_and_neg_flag.neg_flag,
                  curr_basic_circuit.GetMincode());
    bool val_flag = val.Validate();
    std::cout << "ValidateFlag = " << val_flag << std::endl;
    std::cout << "CircuitMatrix:" << std::endl;
    val.PrintCircuitMatrixValue();
    std::cout << std::endl;
    //std::cout << "PrintCircuitMatrix:" << std::endl;
    //curr_circuit.PrintCircuitMatrix();
    std::cout << std::endl;

    std::vector<int> func_value;
    func_ptr->ComputeTableOfValues(func_value);
    for (const auto &elem: func_value) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;

    std::vector<int> circuit_values;
    circuit_values = curr_circuit.GetFinalStringTableOfValues();
    std::cout << "circuit_values:" << std::endl;
    for (const auto &elem: circuit_values) {
        std::cout << elem << ' ';
    }
    std::cout << std::endl;
}
*/

// total_circuits = 616125
// argv[1] = number_of_process
int main(int argc, char const **argv) {
    int _offset = 100000;
    std::ifstream data_base_file("../database_dump_AIG.txt");
    int number_of_process = std::stoi(argv[1]);
    //int number_of_process = 1;
    std::map<int, int> success_map;
    int i = 0;
    if (number_of_process < 7) {
        int max_complexity = 0;
        int id_max_complexity = 0;
        std::string str1 = std::string("../output_files/data_base_xig");
        std::string str2 = std::string(argv[1]);
        //std::string str2 = std::string("1");
        std::string str3 = std::string(".txt");
        std::string out_file_name = str1 + str2 + str3;
        std::ofstream data_xig_base_file(out_file_name);
        for (i = 0; i < (number_of_process - 1) * _offset; ++i) {
            BasicCircuit curr_basic_circuit(data_base_file);
        }
        for (i = (number_of_process - 1) * _offset; i < number_of_process * _offset; ++i) {
            if (i % 1000 == 0) {
                std::cout << "i = " << i << std::endl;
            }
            int diff = 0;
            BasicCircuit curr_basic_circuit(data_base_file);
            int old_complexity = curr_basic_circuit.GetComplexity();
            Poly curr_poly(curr_basic_circuit);
            Function func(curr_poly);
            func.Reduce();
            std::shared_ptr<Function> func_ptr = std::make_shared<Function>(func);
            Circuit curr_circuit(func_ptr);
            IndexAndNegFlag id_and_neg_flag;
            id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(func_ptr);
            Validator val(curr_circuit.GetFunction(), 
                          curr_circuit.GetCircuitMatrix(),
                          id_and_neg_flag.index,
                          id_and_neg_flag.neg_flag,
                          curr_basic_circuit.GetMincode());
            bool val_flag = val.Validate();
            if (val_flag) {
                int new_complexity = val.GetComplexity();
                if (new_complexity < old_complexity) {
                    val.WriteCircuitToFile(data_xig_base_file);
                    diff = old_complexity - new_complexity;
                    AddToMap(success_map, diff);
                    if (new_complexity > max_complexity) {
                        max_complexity = new_complexity;
                        id_max_complexity = i;
                    } 
                } else {
                    curr_basic_circuit.WriteCircuitToFile(data_xig_base_file);
                    if (old_complexity > max_complexity) {
                        max_complexity = old_complexity;
                        id_max_complexity = i;
                    }
                }
            } else {
                std::cout << "IncorrectCircuit" << std::endl;
                std::cout << "i = " << i << std::endl;
            }
        }
        data_xig_base_file << "_______________________" << std::endl;
        data_xig_base_file << "_______________________" << std::endl;
        data_xig_base_file << "_______________________" << std::endl;
        PrintMap(success_map, data_xig_base_file);
        data_xig_base_file << "max_complexity = " << max_complexity << std::endl;
        data_xig_base_file << "id_max_complexity = " << id_max_complexity << std::endl;
        return 0;
    } else {
        int max_complexity = 0;
        int id_max_complexity = 0;
        std::string str1 = std::string("../output_files/data_base_xig");
        std::string str2 = std::string(argv[1]);
        //std::string str2 = std::string("1");
        std::string str3 = std::string(".txt");
        std::string out_file_name = str1 + str2 + str3;
        std::ofstream data_xig_base_file(out_file_name);
        for (i = 0; i < (number_of_process - 1) * _offset; ++i) {
            BasicCircuit curr_basic_circuit(data_base_file);
        }
        for (i = (number_of_process - 1) * _offset; i < 616125; ++i) {
            if (i % 1000 == 0) {
                std::cout << "i = " << i << std::endl;
            }
            int diff = 0;
            BasicCircuit curr_basic_circuit(data_base_file);
            int old_complexity = curr_basic_circuit.GetComplexity();
            Poly curr_poly(curr_basic_circuit);
            Function func(curr_poly);
            func.Reduce();
            std::shared_ptr<Function> func_ptr = std::make_shared<Function>(func);
            Circuit curr_circuit(func_ptr);
            IndexAndNegFlag id_and_neg_flag;
            id_and_neg_flag = curr_circuit.WriteToCircuitMatrix(func_ptr);
            Validator val(curr_circuit.GetFunction(), 
                          curr_circuit.GetCircuitMatrix(),
                          id_and_neg_flag.index,
                          id_and_neg_flag.neg_flag,
                          curr_basic_circuit.GetMincode());
            bool val_flag = val.Validate();
            if (val_flag) {
                int new_complexity = val.GetComplexity();
                if (new_complexity < old_complexity) {
                    val.WriteCircuitToFile(data_xig_base_file);
                    diff = old_complexity - new_complexity;
                    AddToMap(success_map, diff);
                    if (new_complexity > max_complexity) {
                        max_complexity = new_complexity;
                        id_max_complexity = i;
                    } 
                } else {
                    curr_basic_circuit.WriteCircuitToFile(data_xig_base_file);
                    if (old_complexity > max_complexity) {
                        max_complexity = old_complexity;
                        id_max_complexity = i;
                    }
                }
                
            } else {
                std::cout << "IncorrectCircuit" << std::endl;
                std::cout << "i = " << i << std::endl;
            }
        }
        data_xig_base_file << "_______________________" << std::endl;
        data_xig_base_file << "_______________________" << std::endl;
        data_xig_base_file << "_______________________" << std::endl;
        PrintMap(success_map, data_xig_base_file);
        data_xig_base_file << "max_complexity = " << max_complexity << std::endl;
        data_xig_base_file << "id_max_complexity = " << id_max_complexity << std::endl;
        return 0;
    }
}

