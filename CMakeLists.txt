cmake_minimum_required (VERSION 2.6)
project (expr_eval)
# The version number.
set (expr_eval_VERSION_MAJOR 1)
set (expr_eval_VERSION_MINOR 0)
 
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/expr_evalConfig.h.in"
  "${PROJECT_BINARY_DIR}/expr_evalConfig.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find expr_evalConfig.h
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/googletest/")
include_directories("${PROJECT_SOURCE_DIR}/googletest/include")

# for vs2012&googletests
add_definitions(-D_VARIADIC_MAX=10)

set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_RELEASE} /MT")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")

# add the executable
add_executable(expr_eval expr_eval.cpp expression.cpp expression_tokenizer.cpp string_helpers.cpp value.cpp )
add_executable(unittest_expr_eval unittest_expr_eval.cpp expression.cpp expression_tokenizer.cpp string_helpers.cpp value.cpp )

target_link_libraries( expr_eval debug "${PROJECT_SOURCE_DIR}/googletest/Debug/gtest_maind.lib" )
target_link_libraries( expr_eval debug "${PROJECT_SOURCE_DIR}/googletest/Debug/gtestd.lib" )
target_link_libraries( unittest_expr_eval debug "${PROJECT_SOURCE_DIR}/googletest/Debug/gtest_maind.lib" )
target_link_libraries( unittest_expr_eval debug "${PROJECT_SOURCE_DIR}/googletest/Debug/gtestd.lib" )

target_link_libraries( expr_eval optimized "${PROJECT_SOURCE_DIR}/googletest/Release/gtest_main.lib" )
target_link_libraries( expr_eval optimized "${PROJECT_SOURCE_DIR}/googletest/Release/gtest.lib" )
target_link_libraries( unittest_expr_eval optimized "${PROJECT_SOURCE_DIR}/googletest/Release/gtest_main.lib" )
target_link_libraries( unittest_expr_eval optimized "${PROJECT_SOURCE_DIR}/googletest/Release/gtest.lib" )
