cmake_minimum_required(VERSION 3.15)

project(arrtest
  VERSION 2.1.1
  DESCRIPTION "Adequately Reliable Robustness Test Library"
  LANGUAGES CXX)

add_library(arrtest INTERFACE)

target_include_directories(arrtest
  INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
)

target_compile_features(arrtest INTERFACE cxx_std_20)
