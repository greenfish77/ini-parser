##############
# build & test
##############
# * install a latest cmake, and select a platform.
# cmake -G "Visual Studio 17 2022" -A x64
# cmake -G "Visual Studio 16 2019" -A x64
# cmake -G "Visual Studio 15 2017" -A x64
# 
# * build.
# mkdir build
# cd build
# cmake ..
# cmake --build . --config release
# cmake --install . --prefix install
# [ ctest --verbose ]

cmake_minimum_required(VERSION 3.18)

##########
# settings
##########
# project.
project(ini-parser VERSION 1.0)

#################################
# auto git ignore build directory
#################################
if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
    file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()

#######
# c++17
#######
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC_VERSION GREATER_EQUAL 1910)
    # for using `and`, `or` keyword.
    add_compile_options(/permissive-)
endif()

##############
# source files
##############

# include directory.
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/")

# hpp files.
file(GLOB_RECURSE HPP_FILES
    "${INCLUDE_DIR}/*.hpp"
    "${INCLUDE_DIR}/*.h"
)

# cpp files.
set(CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# source files.
set(SOURCE_FILES ${CPP_FILES} ${HPP_FILES})

# file tree.
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HPP_FILES})

############
# definition
############
# compile definitions.
# set(SOME_DEFINITIONS "")

#########
# targets
#########
add_executable(ini-parser ${SOURCE_FILES})

##########
# includes
##########
target_include_directories(ini-parser PRIVATE
    ${INCLUDE_DIR}
)

######
# link
######
# use thread.
find_package(Threads REQUIRED)
target_link_libraries(ini-parser PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    ${CMAKE_DL_LIBS}
)

#########
# install
#########
install(DIRECTORY "${INCLUDE_DIR}" DESTINATION include)
install(TARGETS ini-parser DESTINATION bin)
