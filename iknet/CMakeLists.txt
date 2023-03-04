cmake_minimum_required(VERSION 3.10)

project(iknet VERSION 2.0 LANGUAGES CXX)

# Project files
set(SOURCE_FILES
    src/iknet/AddrInfo.cpp
    src/iknet/Buffer.cpp
    src/iknet/Socket.cpp
    src/iknet/SocketInitializer.cpp
    src/iknet/systemspec.cpp
    src/iknet/TcpListener.cpp
    src/iknet/TcpSocket.cpp
    src/iknet/UdpSocket.cpp
)

set(INCLUDE_FILES
    include/iknet/AddrInfo.hpp
    include/iknet/Buffer.hpp
    include/iknet/iknet_export.hpp
    include/iknet/SenderInfo.hpp
    include/iknet/Socket.hpp
    include/iknet/SocketInitializer.hpp
    include/iknet/SocketPoller.hpp
    include/iknet/systemspec.hpp
    include/iknet/TcpListener.hpp
    include/iknet/TcpSocket.hpp
    include/iknet/UdpRecBuffer.hpp
    include/iknet/UdpRecLength.hpp
    include/iknet/UdpSocket.hpp
)

# Define library
add_library(iknet SHARED ${SOURCE_FILES} ${INCLUDE_FILES})
add_library(iklibs::iknet ALIAS iknet)
target_include_directories(iknet
    PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Dependencies
if(WIN32)
    target_link_libraries(iknet PUBLIC ws2_32 iklibs::ikgen)
else()
    target_link_libraries(iknet PUBLIC iklibs::ikgen)
endif()

# Build options
target_compile_features(iknet PUBLIC cxx_std_20)
set_target_properties(iknet
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

include(CheckIPOSupported)
check_ipo_supported(RESULT ipo_supported)
if(ipo_supported)
  set_target_properties(iknet PROPERTIES INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Export symbols
include(GenerateExportHeader)
GENERATE_EXPORT_HEADER(iknet
             BASE_NAME IKNET
             EXPORT_MACRO_NAME IKNET_EXPORT
             EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/iknet/iknet_export.hpp
             STATIC_DEFINE IKNET_BUILT_AS_STATIC
)

# Install
include(GNUInstallDirs)
install(TARGETS iknet
    EXPORT iknet-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT iknet-targets
    FILE iknetTargets.cmake
    NAMESPACE iklibs::
    DESTINATION cmake
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/iknetConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/iknetConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/iknetConfig.cmake
    INSTALL_DESTINATION cmake
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/iknetConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/iknetConfigVersion.cmake
    DESTINATION cmake
)

# Make the library available in the build tree
export(EXPORT iknet-targets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/iknetTargets.cmake
    NAMESPACE iklibs::
)

export(PACKAGE iknet)
