cmake_minimum_required(VERSION 3.0)

project(iklog)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

set(CMAKE_CXX_STANDARD 11)

# Project settings
set(LIBRARY_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})

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

# Detect and add libpthread
find_package (Threads)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_library(iklog SHARED ${source_files} ${include_files})
target_link_libraries(iklog ${CMAKE_THREAD_LIBS_INIT})
