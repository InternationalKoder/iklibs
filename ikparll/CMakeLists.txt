cmake_minimum_required(VERSION 3.10)

project(ikparll VERSION 1.1 LANGUAGES CXX)

# Project files
set(INCLUDE_FILES
    include/ikparll/ConsumerBase.hpp
    include/ikparll/SingleConsumer.hpp
    include/ikparll/ThreadPool.hpp
)

# Define library
add_library(ikparll INTERFACE)
add_library(iklibs::ikparll ALIAS ikparll)
target_include_directories(ikparll
    INTERFACE
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Build options
target_compile_features(ikparll INTERFACE cxx_std_17)

# Install
include(GNUInstallDirs)
install(TARGETS ikparll
    EXPORT ikparll-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ikparll-targets
    FILE ikparllTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ikparllConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ikparllConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ikparllConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ikparllConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ikparllConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT ikparll-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/ikparllTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE ikparll)
