cmake_minimum_required(VERSION 2.8)

set(NAME "SFML")

project(${NAME})

set(CMAKE_CXX_FLAGS "-O3 -Wall -s -std=c++11")
set(CMAKE_C_FLAGS "-O3 -Wall -s -std=c11")

set(DIRS
    SFMLNetwork/*
    SFMLSystem/*
)

foreach(ELEM ${DIRS})
    file(GLOB_RECURSE FILES ${ELEM}.cpp ${ELEM}.c)
    list(APPEND SRC ${FILES})
endforeach(ELEM)

add_executable(${NAME}.out ${SRC})
add_executable(VxWorksSFML_Example.out VxWorksSFML_Example.cpp)
