cmake_minimum_required(VERSION 3.30)
project(LatinSquareGenerator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Ofast -lgmpxx -lgmp -flto=auto -march=native -funroll-all-loops -fno-stack-protector")

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
