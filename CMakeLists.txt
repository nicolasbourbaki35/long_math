
PROJECT(Template)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

######################################################
# Add boost package. 
#####################################################

#If the Boost library is not installed in the default directory (/usr/lib) uncomment the lines below and give the path to the boost include files and libraries
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "~/Projects/C++/boost_1_55_0/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "~/Projects/C++/boost_1_55_0/lib")

SET(Boost_USE_STATIC_LIBS ON) 
SET(Boost_USE_MULTITHREADED ON)  
SET(Boost_USE_STATIC_RUNTIME OFF) 
FIND_PACKAGE(Boost)
IF (Boost_FOUND)
    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()

######################################################
# Add gtest package. 
#####################################################

#If the gtest library is not installed in the default directory (/usr/lib) uncomment the lines below and give the path to the gtest include files and libraries
SET(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "~/Projects/C++/gtest-1.7.0/include")
SET(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "~/Projects/C++/gtest-1.7.0/lib/.libs")

FIND_PACKAGE(GTest)
IF(GTEST_FOUND)
    INCLUDE_DIRECTORIES(${GTEST_INCLUDE_DIRS})
ENDIF()

######################################################
# Compiler options
#####################################################
set (CMAKE_CXX_FLAGS "-Wall -std=c++11")


######################################################
# Include subdirectories 
#####################################################
ADD_SUBDIRECTORY(long_math_lib)
ADD_SUBDIRECTORY(long_math_test)

