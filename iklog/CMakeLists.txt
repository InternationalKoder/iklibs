cmake_minimum_required(VERSION 3.10)

project(iklog VERSION 0.3 LANGUAGES CXX)

# Project files
set(SOURCE_FILES
    src/iklog/Formatter.cpp
    src/iklog/Log.cpp
    src/iklog/Message.cpp
    src/iklog/NullLog.cpp
    src/iklog/outputs/OstreamWrapper.cpp
    src/iklog/outputs/Output.cpp
    src/iklog/outputs/RollingFileOutput.cpp
)

set(INCLUDE_FILES
    include/iklog/iklog_export.hpp
    include/iklog/Formatter.hpp
    include/iklog/Level.hpp
    include/iklog/Log.hpp
    include/iklog/Message.hpp
    include/iklog/NullLog.hpp
    include/iklog/outputs/OstreamWrapper.hpp
    include/iklog/outputs/Output.hpp
    include/iklog/outputs/RollingFileOutput.hpp
    include/iklog/files/FileSize.hpp
)

# Define project structure for IDE
set_property(TARGET PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/src/iklog" FILES ${SOURCE_FILES})
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}/include/iklog" FILES ${INCLUDE_FILES})

# Define library
add_library(iklog SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(iklibs::iklog ALIAS iklog)
target_include_directories(iklog
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Build options
target_compile_features(iklog PRIVATE cxx_std_17)
set_target_properties(iklog
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(iklog PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Export symbols
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(iklog
             BASE_NAME IKLOG
             EXPORT_MACRO_NAME IKLOG_EXPORT
             EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/iklog/iklog_export.hpp
             STATIC_DEFINE IKLOG_BUILT_AS_STATIC
)

# Install
include(GNUInstallDirs)
install(TARGETS iklog
    EXPORT iklog-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT iklog-targets
    FILE iklogTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/iklogConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/iklogConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/iklogConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/iklogConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/iklogConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT iklog-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/iklogTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE iklog)
