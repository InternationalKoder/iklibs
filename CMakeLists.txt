cmake_minimum_required(VERSION 3.1)

project(iklibs)

option(BUILD_IKLOG "Enable building iklog library" ON)
option(BUILD_IKCONF "Enable building ikconf library" ON)
option(BUILD_EXAMPLES "Enable building examples" ON)

if(BUILD_IKLOG)
    add_subdirectory(iklog)
endif()

if(BUILD_IKCONF)
    add_subdirectory(ikconf)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
