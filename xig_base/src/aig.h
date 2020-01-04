#ifndef AIG
#define AIG

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>

struct Precessors {
    int first_prec;
    bool is_negation_first_prec;
    int second_prec;
    bool is_negation_second_prec;
    bool variable_flag;

public:
    Precessors(int first_prec,
               bool is_negation_first_prec,
               int second_prec,
               bool is_negation_second_prec,
               bool variable_flag);
    bool IsNegationPrec() const;
    bool IsPositivePrec() const;
    bool IsCrossPrec() const;
    bool IsVariable() const;
    bool IsConjunction() const;

    void PrintPrecessors() const;
};

class MyAIG {
    int64_t mincode;
    int64_t complexity;
    int64_t out_number;
    bool negation_out_number;
    std::vector<int> table_of_values;
    std::vector<Precessors> circuit_matrix;

    void ConvertMincodeToVectorInt(const std::string &str, std::vector<int> &vec);
    void ConvertStrToVectorInt(const std::string &str, int startIndex, std::vector<int> &vec);
    void ReadHeader(std::ifstream &aig_in);

public:
    MyAIG();
    //MyAIG(const std::string &inputFile);
    MyAIG(std::ifstream &in_file);
    int64_t GetMincode() const;
    int64_t GetComplexity() const;
    void PrintAIG() const;
    void CountXorStructures(int &sum, std::vector<std::vector<int>> &xorStructures) const;
    bool IsXorStruct1(int prec_1, int prec_2) const;
    bool IsXorStruct2(int prec_1, int prec_2) const;
    //friend std::ostream& operator<< (std::ostream &out, const MyAIG &aig);
};

#endif