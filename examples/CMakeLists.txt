cmake_minimum_required(VERSION 3.10)

project(iklibs-examples VERSION 2.0 LANGUAGES CXX)

# Project files
set(SOURCE_FILES
    src/main.cpp
    src/OtherSubSettings.cpp
    src/Settings.cpp
    src/SubSettings.cpp
    src/SubSettingsArrayItem.cpp
    src/SubSubSettings.cpp
)

set(INCLUDE_FILES
    include/OtherSubSettings.hpp
    include/Settings.hpp
    include/SubSettings.hpp
    include/SubSettingsArrayItem.hpp
    include/SubSubSettings.hpp
)

# Define executable
add_executable(iklibs-examples ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(iklibs-examples
    PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
find_package(iklog 2.0 REQUIRED)
find_package(ikconf 2.0 REQUIRED)
find_package(ikparll 2.0 REQUIRED)
find_package(iklogconf 2.0 REQUIRED)
find_package(iknet 2.0 REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(iklibs-examples PRIVATE iklibs::iklog iklibs::ikconf iklibs::ikparll iklibs::iklogconf iklibs::iknet Threads::Threads)

# Build options
target_compile_features(iklibs-examples PRIVATE cxx_std_17)
set_target_properties(iklibs-examples
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(iklibs-examples PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Install
install(TARGETS iklibs-examples)
install(DIRECTORY resources DESTINATION bin)
