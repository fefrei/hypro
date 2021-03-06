# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# File: CMakeLists.txt                                        #
#                                                             #
# Project name: HyPro                                         #
# Contact:                  #
#	   Stefan Schupp stefan.schupp@cs.rwth-aachen.de      #
#                                                             #
# Author: Stefan Schupp
# Credit: Sebastian Junges, Gereon Kremer
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #


# # # # # # # # # # # # # # # # # # # # # #
# A. Project properties.
# B. Options.
# C. Find system tools and libraries.
# D. Invoke subdirectories.
# E. Export Compile Information.
# # # # # # # # # # # # # # # # # # # # # #

cmake_minimum_required(VERSION 2.8.7 FATAL_ERROR)

# # # # # # # # # # # # # # # # # # # # # #
# A. Project properties
# # # # # # # # # # # # # # # # # # # # # #
project(hypro CXX C)

set(PROJECT_FULLNAME "HyPro")
set(PROJECT_DESCRIPTION "A Toolbox for the Reachability Analysis of Hybrid Systems using Geometric Approximations")
string(TIMESTAMP hypro_MINORVERSION %m)
string(TIMESTAMP hypro_MAJORVERSION %y)
set(hypro_VERSION "${hypro_MAJORVERSION}.${hypro_MINORVERSION}")
set(PROJECT_VERSION "${hypro_VERSION}")
set(hypro_NAME "HyPro" )
set(hypro_DESCRIPTION ${PROJECT_DESCRIPTION})

# path to find own modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake" "${CMAKE_SOURCE_DIR}/cmake/Modules/")
# Include own macros.
include( hypromacros )

# # # # # # # # # # # # # # # # # # # # # #
# B. Options
# # # # # # # # # # # # # # # # # # # # # #

# options
option(STATICLIB_SWITCH "Compile a statically linked version of the library." OFF)
option(HYPRO_USE_COTIRE "Use the COmpilation TIme REducer." OFF)
option(HYPRO_USE_OPENMP "Use OpenMp for parallelization in Eigen3." OFF)
option(HYPRO_USE_PPL "Use PPl polytope wrapper." OFF)
option(HYPRO_LOGGING "Allow logging." OFF)
option(HYPRO_COVERAGE "Enable compiler flags for test coverage diagnostics." OFF)
option(HYPRO_STATISTICS "Create a statistic file." OFF)
option(CLANG_TIDY "Enable usage of clang-tidy" OFF )
option(CREATE_DOC "Allow creation of a doxygen documentation." OFF)
option(DEVELOPER "Compile in developer mode, i.e. more warnings." OFF)
option(EXPORT_TO_CMAKE "Export the project to CMake for easy inclusion" ON)
mark_as_advanced(HYPRO_COVERAGE)
mark_as_advanced(HYPRO_STATISTICS)
mark_as_advanced(CLANG_TIDY)
mark_as_advanced(DEVELOPER)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build, options are: Debug Release" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release")

if(NOT SECONDARY_SOLVER_BACKEND)
	set(SECONDARY_SOLVER_BACKEND "none" CACHE STRING "Secondary backend-solver used after invoking glpk. Options are: none, smtrat, z3, soplex")
endif()
set_property(CACHE SECONDARY_SOLVER_BACKEND PROPERTY STRINGS "none" "smtrat" "z3" "soplex")

if(${SECONDARY_SOLVER_BACKEND} STREQUAL "smtrat")
	set(HYPRO_USE_SMTRAT "ON")
elseif(${SECONDARY_SOLVER_BACKEND} STREQUAL "z3")
	set(HYPRO_USE_Z3 "ON")
elseif(${SECONDARY_SOLVER_BACKEND} STREQUAL "soplex")
	set(HYPRO_USE_SOPLEX "ON")
endif()

configure_file( ${CMAKE_CURRENT_SOURCE_DIR}/.travis/sonar-project.properties.in ${CMAKE_CURRENT_SOURCE_DIR}/.travis/sonar-project.properties )

###########################################################################################
#### EXPORT SETTINGS
# Offer the user the choice of overriding the installation directories

set(INCLUDE_INSTALL_DIR include/ CACHE PATH "Installation directory for header files" )
set(LIB_INSTALL_DIR lib/ CACHE PATH "Installation directory for libraries")
set(BIN_INSTALL_DIR lib/ CACHE PATH "Installation directory for executables")
# path to put in the executables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

if(WIN32 AND NOT CYGWIN)
  set(DEF_INSTALL_CMAKE_DIR CMake)
else()
  set(DEF_INSTALL_CMAKE_DIR lib/cmake/${PROJECT_NAME})
endif()
set(CMAKE_INSTALL_DIR ${DEF_INSTALL_CMAKE_DIR} CACHE PATH "Installation directory for CMake files")

foreach(p LIB BIN INCLUDE CMAKE)
  set(var ${p}_INSTALL_DIR)
  if(NOT IS_ABSOLUTE "${${var}}")
    set(${var} "${CMAKE_INSTALL_PREFIX}/${${var}}")
  endif()
endforeach()

###########################################################################################

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -DGTEST_USE_OWN_TR1_TUPLE=1")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
	if (APPLE)
		message("-- Using clang on apple")
		set(CMAKE_MACOSX_RPATH 1)

		if(${STATICLIB_SWITCH} STREQUAL "OFF")
			set(CMAKE_MACOSX_RPATH 1)
		endif()
	else()
		message("-- Using clang")
	endif()
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
	set(USE_CLANG PARENT SCOPE)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	message("-- Using g++")
	if(NOT HYPRO_COVERAGE)
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	else()
		set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -g -O0 --coverage")
	endif()
	set(CMAKE_CXX_DEBUG_FLAGS "${CMAKE_CXX_DEBUG_FLAGS} -g -O1")
	if(CMAKE_CXX_COMPILER_VERSION VERSION_GREATER 4.9)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=auto")
	endif()
else()
	message("-- Possibly unsupported compiler")
endif()

# general compiler and linker settings

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-attributes -pedantic -fPIC -DBOOST_SPIRIT_USE_PHOENIX_V3=1 -ftemplate-backtrace-limit=0")
if(STATICLIB_SWITCH)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static -pthread -Wl,--whole-archive -lpthread -Wl,--no-whole-archive")
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
	SET(BUILD_SHARED_LIBRARIES OFF)
else()
	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so;.dylib")
	SET(BUILD_SHARED_LIBRARIES ON)
endif()

if(DEVELOPER)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wswitch -Wno-deprecated-declarations -Wempty-body -Wconversion -Wreturn-type -Wparentheses -Wno-format -Wuninitialized -Wunreachable-code -Wunused-function -Wunused-value -Wunused-variable")
endif()

# # # # # # # # # # # # # # # # # # # # # #
# C. Find system tools and libraries.
#
# 1. Required libraries for core.
# 2. Optional libraries for core.
# 3. Development and debug libraries.
# # # # # # # # # # # # # # # # # # # # # #

# # # # # # # # # # # # # # # # # # # # # #
# C.1. Required libraries for core.
# # # # # # # # # # # # # # # # # # # # # #

# initialize lists for include directories and library files
set(${PROJECT_NAME}_INCLUDE_DIRS "" CACHE STRING "Include directories var")

#load_library(${PROJECT_NAME} Boost 1.55.0 COMPONENTS system)
#set(Boost_USE_STATIC_LIBS        ON) # only find static libs
#set(Boost_USE_MULTITHREADED      ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)

load_library(${PROJECT_NAME} GTest 0)
find_package(carl)

add_subdirectory(src/resources)


if(CLANG_TIDY)
	set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
	set(CLANG_TIDY_CHECKS "*")
	set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-llvm-header-guard")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-readability-namespace-comments,-llvm-namespace-comment")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-readability-braces-around-statements,-readability-braces-around-statements")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-readability-else-after-return")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-readability-todo")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-google-runtime-int")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-misc-macro-parentheses")
	#set(CLANG_TIDY_CHECKS "${CLANG_TIDY_CHECKS},-clang-analyzer-alpha.core.CastToStruct")
	set(CLANG_TIDY_CHECKS "-checks='${CLANG_TIDY_CHECKS}'")
	message(STATUS "Enabled checks for clang-tidy: ${CLANG_TIDY_CHECKS}")
endif()

# # # # # # # # # # # # # # # # # # # # # #
# C.2. Optional libraries
# # # # # # # # # # # # # # # # # # # # # #

if(HYPRO_USE_COTIRE)
	# We use cotire, simply include it
	message(STATUS "using cotire")
	include(cotire)
else()
	# We do not use cotire, create dummy function.
	message(STATUS "not using cotire")
	function(cotire)
	endfunction(cotire)
endif()

if(HYPRO_USE_PPL)
	load_library(${PROJECT_NAME} PPL 0)
endif()

if(HYPRO_USE_SMTRAT)
	find_package(smtrat)
endif()

if(HYPRO_USE_Z3)
	load_library(${PROJECT_NAME} z3 0)
endif()

if(HYPRO_USE_SOPLEX)
	load_library(${PROJECT_NAME} soplex 0)
endif()

if(HYPRO_USE_OPENMP)
	load_library(${PROJECT_NAME} OpenMP 0)
	if(OPENMP_FOUND)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
	endif()
endif()

# # # # # # # # # # # # # # # # # # # # # #
# C.3. Development and debug libraries.
# # # # # # # # # # # # # # # # # # # # # #

if(CREATE_DOC)
	add_subdirectory(doc EXCLUDE_FROM_ALL)
endif()

enable_testing()

# # # # # # # # # # # # # # # # # # # # # #
# D. Invoke subdirectories.
# # # # # # # # # # # # # # # # # # # # # #

add_subdirectory(src)
add_subdirectory(src/test)
add_subdirectory(examples EXCLUDE_FROM_ALL)

##############################################################################
#### EXPORT SETTINGS

# # # # # # # # # # # # # # # # # # # # # #
# E. Export Compile Information.
# # # # # # # # # # # # # # # # # # # # # #


include(export)
