cmake_minimum_required(VERSION 3.19)

project(iklibs-examples VERSION ${CMAKE_PROJECT_VERSION} LANGUAGES CXX)

# Project files
set(SOURCE_FILES
    src/ikconf/OtherSubSettings.cpp
    src/ikconf/Settings.cpp
    src/ikconf/SubSettings.cpp
    src/ikconf/SubSettingsArrayItem.cpp
    src/ikconf/SubSubSettings.cpp
    src/ikconfExamples.cpp
    src/iklogconfExamples.cpp
    src/iklogExamples.cpp
    src/iknetExamples.cpp
    src/ikparllExamples.cpp
    src/main.cpp
)

set(INCLUDE_FILES
    include/ikconf/OtherSubSettings.hpp
    include/ikconf/Settings.hpp
    include/ikconf/SubSettings.hpp
    include/ikconf/SubSettingsArrayItem.hpp
    include/ikconf/SubSubSettings.hpp
    include/ikconfExamples.hpp
    include/iklogconfExamples.hpp
    include/iklogExamples.hpp
    include/iknetExamples.hpp
    include/ikparllExamples.hpp
)

# Define executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${INCLUDE_FILES})
target_include_directories(${PROJECT_NAME}
    PRIVATE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE iklibs::iklog iklibs::ikconf iklibs::ikparll iklibs::iklogconf iklibs::iknet Threads::Threads)

# Build options
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Install
install(TARGETS ${PROJECT_NAME})
install(DIRECTORY resources DESTINATION bin)
