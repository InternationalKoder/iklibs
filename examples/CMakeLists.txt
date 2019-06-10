cmake_minimum_required(VERSION 3.1)

project(iklibs-examples)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

set(CMAKE_CXX_STANDARD 17)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

# Project settings
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

file(
    GLOB_RECURSE
    source_files
    src/*
)

file(
    GLOB_RECURSE
    include_files
    include/*
)

include_directories("src")
include_directories("include")

set(EXECUTABLE_NAME "iklibs-examples")

# Add ikconf library headers
include_directories("../iklog/include")
include_directories("../ikconf/include")

add_executable(${EXECUTABLE_NAME} ${source_files} ${include_files})
target_link_libraries(${EXECUTABLE_NAME} PUBLIC iklog ikconf)
