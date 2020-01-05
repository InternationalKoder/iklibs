cmake_minimum_required(VERSION 3.10)

project(iklogconf VERSION 1.0 LANGUAGES CXX)

# Project files
set(SOURCE_FILES
    src/iklogconf/LogConfigurator.cpp
    src/iklogconf/config-model/LogConfiguration.cpp
    src/iklogconf/config-model/LogConfigurationItem.cpp
    src/iklogconf/config-model/LogConfigurationOutput.cpp
    src/iklogconf/config-model/OutputConfigurationItem.cpp
)

set(INCLUDE_FILES
    include/iklogconf/LogConfigurator.hpp
    include/iklogconf/iklogconf_export.hpp
    include/iklogconf/config-model/LogConfiguration.hpp
    include/iklogconf/config-model/LogConfigurationItem.hpp
    include/iklogconf/config-model/LogConfigurationOutput.hpp
    include/iklogconf/config-model/OutputConfigurationItem.hpp
)

# Define project structure for IDE
set_property(TARGET PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/iklogconf" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/iklogconf" FILES ${INCLUDE_FILES})

# Define library
add_library(iklogconf SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(iklibs::iklogconf ALIAS iklogconf)
target_include_directories(iklogconf
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
target_link_libraries(iklogconf PUBLIC iklibs::iklog iklibs::ikconf)

# Build options
target_compile_features(iklogconf PRIVATE cxx_std_17)
set_target_properties(iklogconf
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(iklogconf PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Export symbols
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(iklogconf
             BASE_NAME IKLOGCONF
             EXPORT_MACRO_NAME IKLOGCONF_EXPORT
             EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/iklogconf/iklogconf_export.hpp
             STATIC_DEFINE IKLOGCONF_BUILT_AS_STATIC
)

# Install
include(GNUInstallDirs)
install(TARGETS iklogconf
    EXPORT iklogconf-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT iklogconf-targets
    FILE iklogconfTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/iklogconfConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/iklogconfConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/iklogconfConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/iklogconfConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/iklogconfConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT iklogconf-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/iklogconfTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE iklogconf)
