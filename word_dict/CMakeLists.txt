cmake_minimum_required(VERSION 3.5)

project(word_dict LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(HEADERS
    deps/dtree.hpp
    deps/dtree_utils.hpp
    src/dict/string_dict_utils.h
    src/dict/word_dict.h
    src/main_utils.hpp
)

set(SOURCES
    src/dict/string_dict_utils.cpp
    src/dict/word_dict.cpp
    src/main.cpp
)

add_executable(word_dict ${HEADERS} ${SOURCES})
target_include_directories(word_dict PRIVATE deps src/dict)
