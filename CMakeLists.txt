
project(gdextensionpp LANGUAGES CXX)

cmake_minimum_required(VERSION 3.23)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_policy(SET CMP0076 NEW)

add_library(gdextension-core STATIC)

target_sources(gdextension-core PUBLIC 
    src/core/Interface.cxx
    src/variant/Variant.cxx
    src/variant/String.cxx
    src/variant/StringName.cxx
    src/object/Object.cxx
    src/object/RefCounted.cxx
)
target_include_directories(gdextension-core PUBLIC include/)

add_library(extension SHARED tests/basic/lib.cxx)
target_link_libraries(extension PUBLIC gdextension-core)


set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wshadow -Wnon-virtual-dtor -pedantic -Wpedantic -Wold-style-cast -Wcast-align -Wconversion -Wduplicated-cond -Wduplicated-branches -Wlogical-op -Wnull-dereference -Wdouble-promotion")