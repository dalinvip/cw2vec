cmake_minimum_required(VERSION 2.8.9)
project(cw2vec)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
set (EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

if(MSVC)
 set(LIBS ${LIBS} pthreadVC2)
else()  
 set(LIBS ${LIBS} pthread)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -w  -funroll-loops -O3 -march=native")

add_subdirectory(src)
