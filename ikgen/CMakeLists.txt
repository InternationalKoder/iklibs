cmake_minimum_required(VERSION 3.19)

project(ikgen VERSION 2.0 LANGUAGES CXX)

# Project files
set(INCLUDE_FILES
    include/ikgen/Result.hpp
)

# Define library
add_library(ikgen INTERFACE ${INCLUDE_FILES})
add_library(iklibs::ikgen ALIAS ikgen)
target_include_directories(ikgen
    INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Build options
target_compile_features(ikgen INTERFACE cxx_std_20)

# Install
include(GNUInstallDirs)
install(TARGETS ikgen
    EXPORT ikgen-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ikgen-targets
    FILE ikgenTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ikgenConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ikgenConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ikgenConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ikgenConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ikgenConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT ikgen-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/ikgenTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE ikgen)
