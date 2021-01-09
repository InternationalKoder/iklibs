cmake_minimum_required(VERSION 3.10)

project(ikconf VERSION 1.1 LANGUAGES CXX)

# Options
option(IKCONF_USE_IKLOG "Enable the use of iklog as a logging library for ikconf" ON)

# Project files
set(SOURCE_FILES
    src/ikconf/Configuration.cpp
    src/ikconf/exceptions/ConfigurationException.cpp
    src/ikconf/readers/BaseReader.cpp
    src/ikconf/readers/BufferedFile.cpp
    src/ikconf/readers/JsonReader.cpp
    src/ikconf/readers/PropertiesReader.cpp
)

set(INCLUDE_FILES
    include/ikconf/ikconf_export.hpp
    include/ikconf/lib_conf.hpp
    include/ikconf/Configuration.hpp
    include/ikconf/ConfigurationList.hpp
    include/ikconf/Property.hpp
    include/ikconf/exceptions/ConfigurationException.hpp
    include/ikconf/readers/BaseReader.hpp
    include/ikconf/readers/BufferedFile.hpp
    include/ikconf/readers/JsonReader.hpp
    include/ikconf/readers/PropertiesReader.hpp
)

# Define library
add_library(ikconf SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(iklibs::ikconf ALIAS ikconf)
target_include_directories(ikconf
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
if(IKCONF_USE_IKLOG)
    target_link_libraries(ikconf PUBLIC iklibs::iklog)
endif()

# Build options
target_compile_features(ikconf PRIVATE cxx_std_17)
set_target_properties(ikconf
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(ikconf PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Export symbols
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(ikconf
             BASE_NAME IKCONF
             EXPORT_MACRO_NAME IKCONF_EXPORT
             EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/ikconf/ikconf_export.hpp
             STATIC_DEFINE IKCONF_BUILT_AS_STATIC
)

# Insert library configuration in header file
configure_file(
  "cmake/lib_conf.hpp.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/include/ikconf/lib_conf.hpp"
)


# Install
include(GNUInstallDirs)
install(TARGETS ikconf
    EXPORT ikconf-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ikconf-targets
    FILE ikconfTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/ikconfConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/ikconfConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/ikconfConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/ikconfConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/ikconfConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT ikconf-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/ikconfTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE ikconf)
