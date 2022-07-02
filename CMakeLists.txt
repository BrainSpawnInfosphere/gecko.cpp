cmake_minimum_required(VERSION 3.10.0)
project(gecko VERSION 2022.7.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

cmake_host_system_information(RESULT HOST QUERY HOSTNAME)
cmake_host_system_information(RESULT OSN QUERY OS_NAME)
cmake_host_system_information(RESULT OS_VERSION QUERY OS_RELEASE)
cmake_host_system_information(RESULT PROC QUERY PROCESSOR_DESCRIPTION)

message(STATUS "-------------------------------------")
message(STATUS "  Project: ${PROJECT_NAME}")
message(STATUS "  Version: ${CMAKE_PROJECT_VERSION}")
message(STATUS "  C++ ${CMAKE_CXX_STANDARD}")
message(STATUS "-------------------------------------")
message(STATUS " ${HOST}")
message(STATUS " ${OSN}: ${OS_VERSION}")
message(STATUS " ${PROC}")
message(STATUS "-------------------------------------")

# CPM =========================================================================
set(CPM_DOWNLOAD_LOCATION "${CMAKE_BINARY_DIR}/cmake/CPM.cmake")
set(CPM_VERSION v0.35.1)
if(NOT (EXISTS ${CPM_DOWNLOAD_LOCATION}))
    message(STATUS "Downloading CPM.cmake")
    file(DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/${CPM_VERSION}/CPM.cmake
        ${CPM_DOWNLOAD_LOCATION}
    )
else()
    message(STATUS "CPM ${CPM_VERSION} ready")
endif()
include(${CPM_DOWNLOAD_LOCATION})

# Library =====================================================================
# https://cmake.org/cmake/help/latest/module/ExternalProject.html
# include(ExternalProject)
# ExternalProject_Add(marko-proj
#   GIT_REPOSITORY    https://github.com/gecko-robotics/marko.cpp.git
#   GIT_TAG           main
#   GIT_SHALLOW       TRUE
#   SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/marko/src"
#   BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/marko"
#   # CONFIGURE_COMMAND ""
#   BUILD_COMMAND     "make"
#   INSTALL_COMMAND   ""
#   # TEST_COMMAND      ""
#   PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
# )
#
# SET(MARKO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/marko/src/include)
# SET(MARKO_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/marko)

CPMAddPackage(
    NAME marko
    VERSION 2022.7.1.1
    # TAG latest
    GITHUB_REPOSITORY gecko-robotics/marko.cpp
    # GIT_REPOSITORY https://github.com/gecko-robotics/marko.cpp.git
    OPTIONS
        "EXAMPLES OFF"
)

# if (${marko_ADDED})
#     message(STATUS "marko added ")
# else()
#     message(STATUS "*** couldn't add marko ***")
# endif()
message(STATUS "==> marko CPM added: ${marko_ADDED}")

CPMAddPackage(
    NAME serialcomm
    VERSION 0.0.0
    # TAG latest
    GITHUB_REPOSITORY gecko-robotics/serialcomm
    # GIT_REPOSITORY https://github.com/gecko-robotics/marko.cpp.git
    # OPTIONS
    #     "EXAMPLES OFF"
)

# if (${marko_ADDED})
#     message(STATUS "marko added ")
# else()
#     message(STATUS "*** couldn't add marko ***")
# endif()
message(STATUS "==> serialcomm CPM added: ${serialcomm_ADDED}")

# include(ExternalProject)
# ExternalProject_Add(serial-proj
#   GIT_REPOSITORY    https://github.com/gecko-robotics/serialcomm.git
#   GIT_TAG           main
#   GIT_SHALLOW       TRUE
#   SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/serial/src"
#   BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/serial"
#   # CONFIGURE_COMMAND ""
#   BUILD_COMMAND     "make"
#   INSTALL_COMMAND   ""
#   # TEST_COMMAND      ""
#   PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
# )
#
# SET(SER_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/serial/src/include)
# SET(SER_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/serial)

# Threads ---------------------------------------------------------
find_package(Threads REQUIRED)
if(Threads_FOUND)
    message(STATUS ">> Threads found")
    if(CMAKE_USE_PTHREADS_INIT)
        message(STATUS " -> Using pthreads")
    endif()
    # list(APPEND ALL_LIBS ${CMAKE_THREAD_LIBS_INIT})
else()
    message(WARNING "*** Could not find Threads ***")
endif()


# Library =====================================================================
add_library(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:objlib-gecko>)
add_library(${PROJECT_NAME}-static STATIC $<TARGET_OBJECTS:objlib-gecko>)
set_target_properties(${PROJECT_NAME}-static PROPERTIES OUTPUT_NAME ${PROJECT_NAME})

# if building STATIC library, an unimportant but annoying error appears about
# no symbols
SET(CMAKE_C_ARCHIVE_CREATE   "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_CXX_ARCHIVE_CREATE "<CMAKE_AR> Scr <TARGET> <LINK_FLAGS> <OBJECTS>")
SET(CMAKE_C_ARCHIVE_FINISH   "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")
SET(CMAKE_CXX_ARCHIVE_FINISH "<CMAKE_RANLIB> -no_warning_for_no_symbols -c <TARGET>")

add_library(objlib-gecko
    OBJECT
        src/time.cpp
)
# add_dependencies(${PROJECT_NAME} marko-proj serial-proj)
add_dependencies(objlib-gecko marko serialcomm)
target_include_directories(objlib-gecko
    PUBLIC
        # ${MARKO_INCLUDE_DIR}
        # ${CMAKE_SOURCE_DIR}/include
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)
target_link_libraries(objlib-gecko
    PUBLIC
        marko
        serialcomm
)
# target_link_directories(${PROJECT_NAME}
#     PUBLIC
#         ${MARKO_LIB_DIR}
# )

# Binary ======================================================================
option(EXAMPLES "Examples" ON)
message(STATUS "Building examples is ${EXAMPLES}")
if (EXAMPLES)
    add_subdirectory(examples)
endif()
# set(MAIN main)
# add_executable(${MAIN} main.cpp)
# add_dependencies(${MAIN} marko-proj)
# target_include_directories(${MAIN} PRIVATE ${MARKO_INCLUDE_DIR})
# target_link_libraries(${MAIN} PRIVATE marko)
# target_link_directories(${MAIN} PRIVATE ${MARKO_LIB_DIR})
#
# set(IMU imu)
# add_executable(${IMU} imu.cpp)
# add_dependencies(${IMU} marko-proj serial-proj)
# target_include_directories(${IMU} PRIVATE ${MARKO_INCLUDE_DIR} ${SER_INCLUDE_DIR})
# target_link_libraries(${IMU}
#     PRIVATE
#         marko
#         ${CMAKE_THREAD_LIBS_INIT}
#         serialcomm)
# target_link_directories(${IMU} PRIVATE ${MARKO_LIB_DIR} ${SER_LIB_DIR})
