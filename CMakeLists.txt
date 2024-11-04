cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0167 NEW)
cmake_policy(SET CMP0144 NEW)

project(LatinSquareGenerator VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Ofast -flto=auto -march=native")

# set(Boost_DIR "D:/boost/stage/lib/cmake/Boost-1.86.0")

# find_package(Boost REQUIRED)

# if (Boost_FOUND)
#     include_directories(${Boost_INCLUDE_DIRS})
# endif()

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

# include_directories(${Boost_INCLUDE_DIRS})
# target_link_libraries(LatinSquareGenerator PRIVATE ${Boost_LIBRARIES})
