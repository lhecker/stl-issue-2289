cmake_minimum_required(VERSION 3.19)
project(stl_issue_2289)

set(CMAKE_CXX_STANDARD 20)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Suppressing benchmark's tests" FORCE)

include(FetchContent)
FetchContent_Declare(googlebenchmark GIT_REPOSITORY https://github.com/google/benchmark)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(stl_issue_2289 main.cpp)
target_link_libraries(stl_issue_2289 benchmark::benchmark)
target_link_options(stl_issue_2289 PRIVATE /OPT:REF /OPT:ICF)
set_target_properties(stl_issue_2289 PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
