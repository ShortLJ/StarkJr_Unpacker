# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(StarkJr_Unpacker)

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO )

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
#ROOT_GENERATE_DICTIONARY(G__Sig Sig.h LINKDEF SigLinkDef.h)
#ROOT_GENERATE_DICTIONARY(G__TimeSorter TimeSorter.h LINKDEF TimeSorterLinkDef.h)
#ROOT_GENERATE_DICTIONARY(G__Hit Hit.h LINKDEF HitLinkDef.h)


#---Create a shared library with geneated dictionary
add_library(Sig SHARED src/Sig.cxx inc/Sig.h)
add_library(TimeSorter SHARED src/TimeSorter.cxx inc/TimeSorter.h)
target_link_libraries(TimeSorter Sig)
add_library(HitBuilder SHARED src/HitBuilder.cxx inc/HitBuilder.h)
target_link_libraries(HitBuilder TimeSorter)
target_link_libraries(HitBuilder Hit)
add_library(Hit SHARED src/Hit.cxx inc/Hit.h)
#target_link_libraries(Hit PUBLIC ${ROOT_LIBRARIES} ROOT::RIO ROOT::Net)
target_link_libraries(Hit ${ROOT_LIBRARIES})


#---Create  a main program using the library
add_executable(StarkJr_Unpacker Unpacker.cpp)
target_link_libraries(StarkJr_Unpacker HitBuilder)
target_link_libraries(StarkJr_Unpacker ${ROOT_LIBRARIES})

target_include_directories(StarkJr_Unpacker PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
ROOT_GENERATE_DICTIONARY(Hit_dict Hit.h MODULE StarkJr_Unpacker LINKDEF HitLinkDef.h)
