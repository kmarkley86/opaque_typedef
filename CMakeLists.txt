cmake_minimum_required(VERSION 3.15)
set(ver_major 2)
set(ver_minor 0)
set(ver_patch 0)
set(version ${ver_major}.${ver_minor}.${ver_patch})
set(CMAKE_USER_MAKE_RULES_OVERRIDE ${CMAKE_CURRENT_SOURCE_DIR}/configs.cmake)
project(kmarkley-opaque
  VERSION ${version}
  LANGUAGES CXX
)
include(CTest)
if(BUILD_TESTING)
  if (NOT TARGET tests)
    add_custom_target(tests)
  endif()
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

#
# Configurations
#
get_property(is_multi_config GLOBAL PROPERTY GENERATOR_IS_MULTI_CONFIG)
set(build_types normal profile debug)
if(is_multi_config)
  foreach(type ${build_types})
    if(NOT ${type} IN_LIST CMAKE_CONFIGURATION_TYPES)
      list(APPEND CMAKE_CONFIGURATION_TYPES ${type})
    endif()
  endforeach()
else()
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY
    STRINGS "${build_types}"
  )
  if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE normal CACHE STRING "" FORCE)
  else()
    string(TOLOWER ${CMAKE_BUILD_TYPE} build_type)
    if(NOT build_type IN_LIST build_types)
      message("Build type should be one of: ${build_types}")
      message(FATAL_ERROR "Unknown build type: ${CMAKE_BUILD_TYPE}")
    endif()
  endif()
endif()
message(STATUS "Build type is: ${CMAKE_BUILD_TYPE}")
include(CMakeInitializeConfigs)
cmake_initialize_per_config_variable(CMAKE_CXX_FLAGS           "Flags used by the compiler")
cmake_initialize_per_config_variable(CMAKE_EXE_LINKER_FLAGS    "Flags used by the linker")
cmake_initialize_per_config_variable(CMAKE_SHARED_LINKER_FLAGS "Flags used by the linker")
cmake_initialize_per_config_variable(CMAKE_STATIC_LINKER_FLAGS "Flags used by the linker")
cmake_initialize_per_config_variable(CMAKE_MODULE_LINKER_FLAGS "Flags used by the linker")

#
# Warnings
#
if     (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang" OR
        ${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_compile_options(
    -Wall -Wpedantic -Wextra -Wswitch -Wconversion
    -Wsign-compare -Wsign-conversion
    -Wredundant-decls -Wshadow
    -Wfloat-equal -Wnull-dereference -Wcast-qual
  )
endif()
if     (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
  if(ENABLE_SA)
    set(CMAKE_CXX_CLANG_TIDY "clang-tidy")
  endif()
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
  add_compile_options(
    -Warith-conversion -Wlogical-op
    -Wduplicated-branches -Wduplicated-cond
    -Wcast-align=strict
    -Wno-tautological-compare
  )
endif()

#
# Test definition
#
function(define_simple_test name main lib)
  if(BUILD_TESTING)
    add_executable(${name} EXCLUDE_FROM_ALL ${main})
    target_link_libraries(${name} PRIVATE ${lib})
    add_test(NAME ${name} COMMAND ${name})
    add_dependencies(tests ${name})
    set_tests_properties(${name} PROPERTIES LABELS ${lib})
  endif()
endfunction()

#
# arrtest
#
add_library(arrtest INTERFACE)
#set_target_properties(arrtest PROPERTIES
#  VERSION ${version}
#  SOVERSION ${ver_major}
#)
target_include_directories(arrtest
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(arrtest INTERFACE
  arrtest/result_counter.hpp
  arrtest/test_context.hpp
  arrtest/ostreamable.hpp
  arrtest/type_name.hpp
  arrtest/evaluator.hpp
  arrtest/result_reporter.hpp
  arrtest/test.hpp
  arrtest/tests.hpp
  arrtest/arrtest.hpp
)
set(arrtest_tests
  arrtest/result_counter.test.cpp
  arrtest/test_context.test.cpp
  arrtest/ostreamable.test.cpp
  arrtest/type_name.test.cpp
  arrtest/evaluator.test.cpp
  arrtest/result_reporter.test.cpp
)
foreach(item ${arrtest_tests})
  get_filename_component(name ${item} NAME_WE)
  define_simple_test(${name} ${item} arrtest)
endforeach()

#
# opaque
#
add_library(opaque INTERFACE)
#set_target_properties(opaque PROPERTIES
#  VERSION ${version}
#  SOVERSION ${ver_major}
#)
target_include_directories(opaque
  INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
)
target_sources(opaque INTERFACE
  opaque/storage.hpp
  opaque/ostream.hpp
  opaque/type_traits.hpp
  opaque/convert.hpp
  opaque/binop_function.hpp
  opaque/binop_overload.hpp
  opaque/binop_inherit.hpp
  opaque/numeric_typedef.hpp
  opaque/position_typedef.hpp
  opaque/inconvertibool.hpp
  opaque/safer_string_typedef.hpp
  opaque/string_typedef.hpp
  opaque/hash.hpp
  opaque/tracing_base.test.hpp
  opaque/opaque.hpp
)
set(opaque_tests
  opaque/storage.test.cpp
  opaque/ostream.test.cpp
  opaque/type_traits.test.cpp
  opaque/convert.test.cpp
  opaque/binop_function.test.cpp
  opaque/binop_overload.test.cpp
  opaque/binop_inherit.test.cpp
  opaque/numeric_typedef.test.cpp
  opaque/inconvertibool.test.cpp
  opaque/safer_string_typedef.test.cpp
  opaque/string_typedef.test.cpp
  opaque/hash.test.cpp
)
foreach(item ${opaque_tests})
  get_filename_component(name ${item} NAME_WE)
  define_simple_test(${name} ${item} opaque)
endforeach()
set(opaque_bins
  example/demo_numeric_typedef.cpp
  example/tutorial.cpp
)
foreach(item ${opaque_bins})
  get_filename_component(name ${item} NAME_WE)
  add_executable(${name} ${item})
  target_link_libraries(${name} opaque)
endforeach()
