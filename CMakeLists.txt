cmake_minimum_required(VERSION 3.15)

project(LatinSquareGenerator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB_RECURSE LatinSquareGenerator_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
file(GLOB_RECURSE LatinSquareGenerator_HEADERS CONFIGURE_DEPENDS "src/*.hpp")

set(LatinSquareGenerator_INCLUDE_DIRS "")

foreach(_headerFile ${LatinSquareGenerator_HEADERS})
    get_filename_component(_dir ${_headerFile} PATH)
    list(APPEND LatinSquareGenerator_INCLUDE_DIRS ${_dir})
endforeach()

list(REMOVE_DUPLICATES LatinSquareGenerator_INCLUDE_DIRS)

add_executable(LatinSquareGenerator ${LatinSquareGenerator_SOURCES})

target_include_directories(LatinSquareGenerator PRIVATE ${LatinSquareGenerator_INCLUDE_DIRS})
