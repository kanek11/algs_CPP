
#new version of cmake
cmake_minimum_required(VERSION 3.13)

project(algs_CPP)   

#source
add_executable(algs_CPP ${CMAKE_CURRENT_SOURCE_DIR}/containers/vector_test.cpp)  

#header
target_include_directories(algs_CPP PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/containers)