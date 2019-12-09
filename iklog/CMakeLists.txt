cmake_minimum_required(VERSION 3.0)

include(GenerateExportHeader)

project(iklog)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

set(CMAKE_CXX_STANDARD 17)

# Project settings
set(LIBRARY_OUTPUT_PATH lib/)

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

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_library(iklog SHARED ${source_files} ${include_files})

GENERATE_EXPORT_HEADER(iklog
             BASE_NAME IKLOG
             EXPORT_MACRO_NAME IKLOG_EXPORT
             EXPORT_FILE_NAME include/iklog/iklog_export.hpp
             STATIC_DEFINE IKLOG_BUILT_AS_STATIC
)

install(TARGETS iklog)
install(DIRECTORY include DESTINATION .)
