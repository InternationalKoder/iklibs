cmake_minimum_required(VERSION 3.0)

project(ikparll)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3")
endif()

set(CMAKE_CXX_STANDARD 11)

file(
    GLOB_RECURSE
    include_files
    include/*
)

include_directories("include")

add_library(ikparll INTERFACE)
target_sources(ikparll INTERFACE ${include_files})
target_include_directories(ikparll INTERFACE include/)

install(TARGETS ikparll)
install(DIRECTORY include DESTINATION .)
