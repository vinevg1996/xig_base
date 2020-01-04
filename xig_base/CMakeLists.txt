cmake_minimum_required(VERSION 3.5)

project(AIG)

#set(CMAKE_CXX_COMPILER "g++-7")
set (CMAKE_CXX_STANDARD 17)

#add_library(aig src/aig.cpp src/aig.h)
add_library(function src/basic_circuit.cpp src/basic_circuit.h src/polynom.cpp src/polynom.h src/function.cpp src/function.h src/implementation.cpp src/implementation.h src/validator.cpp src/validator.h)

add_executable(main_xig src/main_xig.cpp)

#target_link_libraries(main aig function)
target_link_libraries(main_xig function)
