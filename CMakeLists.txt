cmake_minimum_required(VERSION 3.19)

project(iklibs VERSION 2.0 LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX -d)

option(BUILD_IKGEN "Enable building ikgen library" ON)
option(BUILD_IKCONF "Enable building ikconf library" ON)
option(BUILD_IKLOG "Enable building iklog library" ON)
option(BUILD_IKLOGCONF "Enable building iklogconf library" ON)
option(BUILD_IKNET "Enable building iknet library" ON)
option(BUILD_IKPARLL "Enable building ikparll library" ON)
option(BUILD_EXAMPLES "Enable building examples" ON)

if(BUILD_IKGEN)
    add_subdirectory(ikgen)
endif()

if(BUILD_IKCONF)
    add_subdirectory(ikconf)
endif()

if(BUILD_IKLOG)
    add_subdirectory(iklog)
endif()

if(BUILD_IKLOGCONF)
    add_subdirectory(iklogconf)
endif()

if(BUILD_IKNET)
    add_subdirectory(iknet)
endif()

if(BUILD_IKPARLL)
    add_subdirectory(ikparll)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

