cmake_minimum_required(VERSION 3.19)

project(iklogconf VERSION ${CMAKE_PROJECT_VERSION} LANGUAGES CXX)
string(TOUPPER ${PROJECT_NAME} UPPER_PROJECT_NAME)

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
    include/iklogconf/Warning.hpp
    include/iklogconf/iklogconf_export.hpp
    include/iklogconf/config-model/LogConfiguration.hpp
    include/iklogconf/config-model/LogConfigurationItem.hpp
    include/iklogconf/config-model/LogConfigurationOutput.hpp
    include/iklogconf/config-model/OutputConfigurationItem.hpp
)

# Define library
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(iklibs::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
target_link_libraries(${PROJECT_NAME} PUBLIC iklibs::iklog iklibs::ikconf)

# Build options
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(${PROJECT_NAME} PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Export symbols
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(${PROJECT_NAME}
             BASE_NAME ${UPPER_PROJECT_NAME}
             EXPORT_MACRO_NAME ${UPPER_PROJECT_NAME}_EXPORT
             EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/${PROJECT_NAME}/${PROJECT_NAME}_export.hpp
             STATIC_DEFINE ${UPPER_PROJECT_NAME}_BUILT_AS_STATIC
)

# Install
include(GNUInstallDirs)
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/${PROJECT_NAME}Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT ${PROJECT_NAME}-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Targets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE ${PROJECT_NAME})
