cmake_minimum_required(VERSION 3.12)
project(pystl VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Python3 REQUIRED COMPONENTS Interpreter Development)
find_package(Threads REQUIRED)
find_package(pybind11 REQUIRED)

pybind11_add_module(${PROJECT_NAME} pystl/TreeDict.cc)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall)
target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
