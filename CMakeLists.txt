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


# Library =====================================================================
# https://cmake.org/cmake/help/latest/module/ExternalProject.html
include(ExternalProject)
ExternalProject_Add(marko-proj
  GIT_REPOSITORY    https://github.com/gecko-robotics/marko.cpp.git
  GIT_TAG           main
  GIT_SHALLOW       TRUE
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/marko/src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/marko"
  # CONFIGURE_COMMAND ""
  BUILD_COMMAND     "make"
  INSTALL_COMMAND   ""
  # TEST_COMMAND      ""
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

SET(MARKO_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/marko/src/include)
SET(MARKO_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/marko)

include(ExternalProject)
ExternalProject_Add(serial-proj
  GIT_REPOSITORY    https://github.com/gecko-robotics/serialcomm.git
  GIT_TAG           main
  GIT_SHALLOW       TRUE
  SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/serial/src"
  BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/serial"
  # CONFIGURE_COMMAND ""
  BUILD_COMMAND     "make"
  INSTALL_COMMAND   ""
  # TEST_COMMAND      ""
  PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
)

SET(SER_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/serial/src/include)
SET(SER_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/serial)

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
add_library(${PROJECT_NAME}
    src/time.cpp
)
add_dependencies(${PROJECT_NAME} marko-proj serial-proj)
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${MARKO_INCLUDE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)
target_link_libraries(${PROJECT_NAME}
    PRIVATE
        marko
)
target_link_directories(${PROJECT_NAME}
    PRIVATE
        ${MARKO_LIB_DIR}
)

# Binary ======================================================================
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
