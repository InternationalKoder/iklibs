cmake_minimum_required(VERSION 3.1)

project(iklibs)

option(BUILD_IKLOG "Enable building iklog library" ON)

if(BUILD_IKLOG)
    add_subdirectory(iklog)
endif()
