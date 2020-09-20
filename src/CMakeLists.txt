#cmake 최소버전 지정
cmake_minimum_required(VERSION 2.8)

#PROJECT이름 지정
PROJECT(CubicSolver)

#컴파일러 지정
set(CMAKE_CXX_COMPILER g++)

#실행파일 저장경로 지정
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin)



#컴파일러 옵션 지정
add_definitions("-Wall -std=c++14")


#빌드
add_executable(CubicSolverTest
 CubicSolverTest.cpp CubicSolver.hpp
)