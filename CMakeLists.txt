cmake_minimum_required(VERSION 3.22)
project(json_type)

set(CMAKE_CXX_STANDARD 20)

add_executable(json_type value.h json.h parser.h generator.h)
