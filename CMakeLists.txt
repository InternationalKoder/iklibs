cmake_minimum_required(VERSION 3.10)

project(iklibs VERSION 0.3 LANGUAGES CXX)

set(CMAKE_DEBUG_POSTFIX -d)

option(BUILD_IKLOG "Enable building iklog library" ON)
option(BUILD_IKCONF "Enable building ikconf library" ON)
option(BUILD_IKPARLL "Enable building ikparll library" ON)
option(BUILD_EXAMPLES "Enable building examples" ON)

if(BUILD_IKLOG)
    add_subdirectory(iklog)
endif()

if(BUILD_IKCONF)
    add_subdirectory(ikconf)
endif()

if(BUILD_IKPARLL)
    add_subdirectory(ikparll)
endif()

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
