cmake_minimum_required(VERSION 3.20)
project(
  scheduler
  VERSION 0.0.1
  LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_COMPILER "clang++")
set(CMAKE_BUILD_TYPE "Debug")
set(project_version "${${PROJECT_NAME}_VERSION}")

message(STATUS "Project '${PROJECT_NAME}', version: '${project_version}'")

option(SCHEDULER_OPT_BUILD_UNITTESTS "Build all scheduler unittests" ON)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  # for debug of stl structure while using clang compile
  add_compile_options(-fstandalone-debug)
endif()

#------------------------------------------------------------
# CMake helpers begin
#------------------------------------------------------------
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

include(CTest)
enable_testing()
find_package(GTest REQUIRED CONFIG)

#------------------------------------------------------------
# CMake helpers end
#------------------------------------------------------------

#------------------------------------------------------------
# lib make begin
#------------------------------------------------------------

set(FLOW_SRCS)

set(STATIC_LIB_NAME ${PROJECT_NAME})

# flowLang lib
file(GLOB_RECURSE FLOW_SRCS
    lib/*/*.cc
  )
add_library(${STATIC_LIB_NAME} STATIC ${FLOW_SRCS})
target_link_libraries(${STATIC_LIB_NAME})
target_include_directories(${STATIC_LIB_NAME}
  PUBLIC
  ${CMAKE_SOURCE_DIR}/include)

#------------------------------------------------------------
# lib make end
#------------------------------------------------------------

if (SCHEDULER_OPT_BUILD_UNITTESTS)
  add_subdirectory(unittests #[[EXCLUDE_FROM_ALL]])
endif()